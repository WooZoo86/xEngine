#include "Shader.h"

#include "window/Window.h"

namespace xEngine {

static struct {
  const char *key;
  const char *value[static_cast<uint8>(GraphicsType::kCount)];
} const g_replacement_table[] = {
    { "in", { "in", "" } },
    { "out", { "out", "" } },
    { "uniform", { "uniform", "" } },
    { "bool", { "bool", "bool" } },
    { "int", { "int", "int" } },
    { "float", { "float", "float" } },
    { "float2", { "vec2", "float2" } },
    { "float3", { "vec3", "float3" } },
    { "float4", { "vec4", "float4" } },
    { "float2x2", { "mat2", "float2x2" } },
    { "float3x3", { "mat3", "float3x3" } },
    { "float4x4", { "mat4", "float4x4" } },
    { "sampler2d", { "sampler2D", "Texture2D" } },
    { "vs_position", { "gl_Position", "fuck" } },
    { "fs_color", { "outColor", "fuck" } },
    { "vs", { "void main()", "void main()" } },
    { "fs", { "void main()", "void main()" } },
};

static bool IsEqual(const char *left, const char *right, size_t left_size, size_t right_size) {
  return strncmp(left, right, eastl::max(left_size, right_size)) == 0;
}

static void Trim(const char *&begin, const char *&end) {
  x_assert(begin < end);
  while (begin < end && (*begin == ' ' || *begin == '\t' || *begin == '\r')) ++begin;
  while (begin < end && (*(end - 1) == ' ' || *(end - 1) == '\t') || *(end - 1) == '\r') --end;
}

static eastl::string ProcessMulExpression(GraphicsType type,
                                          const char *operation_begin, const char *operation_end,
                                          const char *arguments_begin, const char *arguments_end) {
  eastl::string out;
  if (IsEqual(operation_begin, "mul", operation_end - operation_begin, strlen("mul"))) {
    auto left_begin = arguments_begin, left_end = arguments_begin;
    while (*reinterpret_cast<const uint16 *>(left_end) != '||' && left_end != arguments_end) ++left_end;

    auto right_begin = left_end + 2, right_end = left_end + 2;
    while (*reinterpret_cast<const uint16 *>(right_end) != '||' && right_end != arguments_end) ++right_end;

    Trim(left_begin, left_end);
    Trim(right_begin, right_end);

    if (type == GraphicsType::kOpenGL3) {
      out.append(left_begin, left_end).append(" * ").append(right_begin, right_end);
    } else if (type == GraphicsType::kD3D11) {
      out.append("mul(").append(left_begin, left_end).append(", ").append(right_begin, right_end).append(")");
    }
  }
  return out;
}

static eastl::string ProcessSampleExpression(GraphicsType type,
                                             const char *operation_begin, const char *operation_end,
                                             const char *arguments_begin, const char *arguments_end) {
  eastl::string out;
  if (IsEqual(operation_begin, "sample", operation_end - operation_begin, strlen("sample"))) {
    auto left_begin = arguments_begin, left_end = arguments_begin;
    while (*reinterpret_cast<const uint16 *>(left_end) != '||' && left_end != arguments_end) ++left_end;

    auto right_begin = left_end + 2, right_end = left_end + 2;
    while (*reinterpret_cast<const uint16 *>(right_end) != '||' && right_end != arguments_end) ++right_end;

    Trim(left_begin, left_end);
    Trim(right_begin, right_end);

    if (type == GraphicsType::kOpenGL3) {
      out.append("texture(").append(left_begin, left_end).append(", ").append(right_begin, right_end).append(")");
    } else if (type == GraphicsType::kD3D11) {
      out.append("texture(").append(left_begin, left_end).append(", ").append(right_begin, right_end).append(")");
    }
  }
  return out;
}

static eastl::string (*const g_expression_processor[])(GraphicsType, const char *, const char *, const char *, const char *) = {
    ProcessMulExpression,
    ProcessSampleExpression,
};

static eastl::string FindBlock(GraphicsPipelineStage stage, const char *begin, const char *end) {
  x_assert(begin < end);
  eastl::string out;
  auto in_block = false;
  auto write_flag = false;
  auto pointer = begin;
  while (pointer != end) {
    auto line_begin = pointer, line_end = pointer;
    while (*line_end != '\n' && line_end < end) ++line_end;
    if (line_begin != line_end) {
      auto trimed_line_begin = line_begin, trimed_line_end = line_end;
      Trim(trimed_line_begin, trimed_line_end);
      if (*trimed_line_begin == '@') {
        auto operation_begin = trimed_line_begin + 1, operation_end = trimed_line_begin + 1;
        while (*operation_end != ' ' && operation_end < trimed_line_end) ++operation_end;
        Trim(operation_begin, operation_end);
        auto block_name_begin = operation_end + 1, block_name_end = operation_end + 1;
        while (*block_name_end != ' ' && block_name_end < trimed_line_end) ++block_name_end;
        Trim(block_name_begin, block_name_end);
        const char *token = nullptr;
        switch (stage) {
          case GraphicsPipelineStage::kVertexShader: token = "vertex"; break;
          case GraphicsPipelineStage::kFragmentShader: token = "fragment"; break;
        }
        in_block = IsEqual(block_name_begin, token, block_name_end - block_name_begin, strlen(token));
        write_flag = IsEqual(operation_begin, "begin", operation_end - operation_begin, strlen("begin"));
      } else {
        if (in_block && write_flag) {
          out.append(line_begin, line_end).push_back('\n');
        }
      }
    }
    pointer = line_end + 1;
  }
  return out;
}

static eastl::string ProcessReplacement(GraphicsType type, const char *begin, const char *end) {
  x_assert(begin < end);
  eastl::string out;
  if (type == GraphicsType::kOpenGL3) {
    out.append("#version 410 core\n");
  }
  auto pointer = begin;
  const char *variable_begin = nullptr, *variable_end = nullptr;
  while (pointer != end) {
    auto mark_pointer = reinterpret_cast<const uint16 *>(pointer);
    if (*mark_pointer == '-<') {
      pointer += 2;
      variable_begin = pointer;
    } else if (*mark_pointer == '>-') {
      variable_end = pointer;
      pointer += 2;
      Trim(variable_begin, variable_end);
      for (auto index = 0; index < EAArrayCount(g_replacement_table); ++index) {
        auto max_length = eastl::max(strlen(g_replacement_table[index].key), static_cast<size_t>(variable_end - variable_begin));
        if (strncmp(variable_begin, g_replacement_table[index].key, max_length) == 0) {
          out.append(g_replacement_table[index].value[static_cast<uint8>(type)]);
          break;
        }
      }
      variable_begin = variable_end = nullptr;
    } else {
      if (variable_begin == nullptr && variable_end == nullptr) {
        out.push_back(*pointer);
      }
      ++pointer;
    }
  }
  return out;
}

static eastl::string ProcessExpression(GraphicsType type, const char *begin, const char *end) {
  x_assert(begin < end);
  eastl::string out;
  auto pointer = begin;
  const char *expression_begin = nullptr, *expression_end = nullptr;
  while (pointer != end) {
    auto mark_pointer = reinterpret_cast<const uint16 *>(pointer);
    if (*mark_pointer == '-{') {
      pointer += 2;
      expression_begin = pointer;
    } else if (*mark_pointer == '}-') {
      expression_end = pointer;
      pointer += 2;

      Trim(expression_begin, expression_end);

      auto operation_begin = expression_begin, operation_end = expression_begin;
      while (*reinterpret_cast<const uint16 *>(operation_end) != '||' && operation_end != expression_end) ++operation_end;

      auto arguments_begin = operation_end + 2, arguments_end = expression_end;

      Trim(operation_begin, operation_end);
      Trim(arguments_begin, arguments_end);

      for (auto index = 0; index < EAArrayCount(g_expression_processor); ++index) {
        auto processed = g_expression_processor[index](type, operation_begin, operation_end, arguments_begin, arguments_end);
        if (!processed.empty()) {
          out.append(processed);
          break;
        }
      }

      expression_begin = expression_end = nullptr;
    } else {
      if (expression_begin == nullptr && expression_end == nullptr) {
        out.push_back(*pointer);
      }
      ++pointer;
    }
  }
  return out;
}

static eastl::string Generate(GraphicsType type, GraphicsPipelineStage stage, DataPtr data) {
  auto begin = static_cast<const char *>(data->buffer());
  auto end = begin + data->size();

  auto shader = FindBlock(stage, begin, end);
  
  if (!shader.empty()) {
    shader = ProcessReplacement(type, shader.begin(), shader.end());
    shader = ProcessExpression(type, shader.begin(), shader.end());
  }

  Log::GetInstance().Info("=========\n%s\n", shader.c_str());

  return shader;
}

ShaderPtr Shader::Parse(ResourceID window, DataPtr data) {
  x_assert(window != kInvalidResourceID);
  
  auto shader = Shader::Create();
  shader->window_id_ = window;
  
  auto type = Window::GetInstance().GetGraphics(window)->config().type;

  shader->vertex_ = Generate(type, GraphicsPipelineStage::kVertexShader, data);
  shader->fragment_ = Generate(type, GraphicsPipelineStage::kFragmentShader, data);

  return shader;
}

void Shader::Initialize() {
  x_assert(resource_id_ == kInvalidResourceID);
  resource_id_ = Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Create(ShaderConfig::FromString(vertex_, fragment_));
}

void Shader::Finalize() {
  x_assert(resource_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->resource_manager()->Destroy(resource_id_);
}

void Shader::Apply() {
  x_assert(resource_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->ApplyShader(resource_id_);
}

void Shader::UpdateResourceData(const eastl::string &name, DataPtr data) {
  x_assert(resource_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderResourceData(resource_id_, name, data);
}

void Shader::UpdateResourceTexture(const eastl::string &name, ResourceID texture_id) {
  x_assert(resource_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderResourceTexture(resource_id_, name, texture_id);
}

void Shader::UpdateResourceSampler(const eastl::string &name, ResourceID sampler_id) {
  x_assert(resource_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderResourceSampler(resource_id_, name, sampler_id);
}

void Shader::UpdateResourceBlock(const eastl::string &name, ResourceID uniform_buffer_id) {
  x_assert(resource_id_ != kInvalidResourceID);
  Window::GetInstance().GetGraphics(window_id_)->renderer()->UpdateShaderResourceBlock(resource_id_, name, uniform_buffer_id);
}

} // namespace xEngine