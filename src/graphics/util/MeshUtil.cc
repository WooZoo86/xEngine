#include "MeshUtil.h"

namespace xEngine {

MeshUtil MeshUtil::Cube(bool uv) {
  MeshUtil mesh;
  mesh.config_.layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat3);
  if (uv) {
    mesh.config_.layout.AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2);
  }
  mesh.config_.vertex_count = 24;
  mesh.config_.vertex_usage = BufferUsage::kImmutable;
  mesh.config_.index_count = 36;
  mesh.config_.index_type = IndexFormat::kUint16;
  mesh.config_.index_usage = BufferUsage::kImmutable;
  mesh.BeginVertex();
  mesh.BeginIndex();
  size_t vertex_count = 0;
  size_t index_count = 0;
  // front
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, 0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, 0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, -0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, -0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Quad16(index_count, 0, 1, 2, 3);
  index_count += 6;
  // back
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, 0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, 0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, -0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, -0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Quad16(index_count, 4, 5, 6, 7);
  index_count += 6;
  // top
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, 0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, 0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, 0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, 0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Quad16(index_count, 8, 9, 10, 11);
  index_count += 6;
  // bottom
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, -0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, -0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, -0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, -0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Quad16(index_count, 12, 13, 14, 15);
  index_count += 6;
  // left
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, 0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, 0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, -0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, -0.5f, -0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Quad16(index_count, 16, 17, 18, 19);
  index_count += 6;
  // right
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, 0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, 0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 0.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, -0.5f, -0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 1.0f, 1.0f);
  }
  ++vertex_count;
  mesh.Vertex(VertexElementSemantic::kPosition, vertex_count, 0.5f, -0.5f, 0.5f);
  if (uv) {
    mesh.Vertex(VertexElementSemantic::kTexcoord0, vertex_count, 0.0f, 1.0f);
  }
  mesh.Quad16(index_count, 20, 21, 22, 23);
  mesh.EndVertex();
  mesh.EndIndex();
  return mesh;
}

// https://gamedevdaily.io/four-ways-to-create-a-mesh-for-a-sphere-d7956b825db4
MeshUtil MeshUtil::Sphere(bool uv, size_t divisions) {
  return MeshUtil();
}

MeshUtil MeshUtil::Capsule() {
  return MeshUtil();
}

MeshUtil MeshUtil::Cylinder() {
  return MeshUtil();
}

MeshUtil MeshUtil::Plane(bool uv) {
  MeshUtil mesh;
  mesh.config_.layout.AddElement(VertexElementSemantic::kPosition, VertexElementFormat::kFloat3);
  if (uv) {
    mesh.config_.layout.AddElement(VertexElementSemantic::kTexcoord0, VertexElementFormat::kFloat2);
  }
  mesh.config_.vertex_count = 4;
  mesh.config_.vertex_usage = BufferUsage::kImmutable;
  mesh.config_.index_count = 6;
  mesh.config_.index_type = IndexFormat::kUint16;
  mesh.config_.index_usage = BufferUsage::kImmutable;
  mesh.BeginVertex()
      .Vertex(VertexElementSemantic::kPosition, 0, -0.5f, 0.0f, -0.5f)
      .Vertex(VertexElementSemantic::kTexcoord0, 0, 0.0f, 0.0f)
      .Vertex(VertexElementSemantic::kPosition, 1, 0.5f, 0.0f, -0.5f)
      .Vertex(VertexElementSemantic::kTexcoord0, 1, 1.0f, 0.0f)
      .Vertex(VertexElementSemantic::kPosition, 2, 0.5f, 0.0f, 0.5f)
      .Vertex(VertexElementSemantic::kTexcoord0, 2, 1.0f, 1.0f)
      .Vertex(VertexElementSemantic::kPosition, 3, -0.5f, 0.0f, 0.5f)
      .Vertex(VertexElementSemantic::kTexcoord0, 3, 0.0f, 1.0f)
      .EndVertex()
      .BeginIndex()
      .Quad16(0, 0, 1, 2, 3)
      .EndIndex();
  return mesh;
}

MeshUtil &MeshUtil::BeginVertex() {
  x_assert(vertex_data_ == nullptr && config_.vertex_count > 0 && config_.layout.size > 0);
  vertex_data_ = Data::Create(config_.vertex_count * config_.layout.size);
  return *this;
}

MeshUtil &MeshUtil::Vertex(VertexElementSemantic semantic, size_t index, float32 x) {
  x_assert(vertex_data_ != nullptr);
  x_assert(index < config_.vertex_count);
  for (auto i = 0; i < config_.layout.element_count; ++i) {
    if (config_.layout.elements[i].semantic == semantic) {
      auto raw = static_cast<uint8 *>(vertex_data_->buffer()) + index * config_.layout.size + config_.layout.elements[i].offset;
      if (config_.layout.elements[i].format == VertexElementFormat::kFloat1) {
        auto pointer = reinterpret_cast<float32 *>(raw);
        *pointer = x;
        return *this;
      } else {
        x_error("unsupported semantic format\n");
      }
    }
  }
  x_error("no semantic in layout\n");
  return *this;
}

MeshUtil &MeshUtil::Vertex(VertexElementSemantic semantic, size_t index, float32 x, float32 y) {
  x_assert(vertex_data_ != nullptr);
  x_assert(index < config_.vertex_count);
  for (auto i = 0; i < config_.layout.element_count; ++i) {
    if (config_.layout.elements[i].semantic == semantic) {
      auto raw = static_cast<uint8 *>(vertex_data_->buffer()) + index * config_.layout.size + config_.layout.elements[i].offset;
      if (config_.layout.elements[i].format == VertexElementFormat::kFloat2) {
        auto pointer = reinterpret_cast<float32 *>(raw);
        *pointer++ = x;
        *pointer = y;
        return *this;
      } else if (config_.layout.elements[i].format == VertexElementFormat::kShort2) {
        auto pointer = reinterpret_cast<int16 *>(raw);
        *pointer++ = Convert<int16>(x);
        *pointer = Convert<int16>(y);
        return *this;
      } else if (config_.layout.elements[i].format == VertexElementFormat::kShort2Normalized) {
        auto pointer = reinterpret_cast<int16 *>(raw);
        *pointer++ = ConvertNormalized<int16>(x);
        *pointer = ConvertNormalized<int16>(y);
        return *this;
      } else {
        x_error("unsupported semantic format\n");
      }
    }
  }
  x_error("no semantic in layout\n");
  return *this;
}

MeshUtil &MeshUtil::Vertex(VertexElementSemantic semantic, size_t index, float32 x, float32 y, float32 z) {
  x_assert(vertex_data_ != nullptr);
  x_assert(index < config_.vertex_count);
  for (auto i = 0; i < config_.layout.element_count; ++i) {
    if (config_.layout.elements[i].semantic == semantic) {
      auto raw = static_cast<uint8 *>(vertex_data_->buffer()) + index * config_.layout.size + config_.layout.elements[i].offset;
      if (config_.layout.elements[i].format == VertexElementFormat::kFloat3) {
        auto pointer = reinterpret_cast<float32 *>(raw);
        *pointer++ = x;
        *pointer++ = y;
        *pointer = z;
        return *this;
      } else {
        x_error("unsupported semantic format\n");
      }
    }
  }
  x_error("no semantic in layout\n");
  return *this;
}

MeshUtil &MeshUtil::Vertex(VertexElementSemantic semantic, size_t index, float32 x, float32 y, float32 z, float32 w) {
  x_assert(vertex_data_ != nullptr);
  x_assert(index < config_.vertex_count);
  for (auto i = 0; i < config_.layout.element_count; ++i) {
    if (config_.layout.elements[i].semantic == semantic) {
      auto raw = static_cast<uint8 *>(vertex_data_->buffer()) + index * config_.layout.size + config_.layout.elements[i].offset;
      if (config_.layout.elements[i].format == VertexElementFormat::kFloat4) {
        auto pointer = reinterpret_cast<float32 *>(raw);
        *pointer++ = x;
        *pointer++ = y;
        *pointer++ = z;
        *pointer = w;
        return *this;
      } else if (config_.layout.elements[i].format == VertexElementFormat::kByte4) {
        auto pointer = reinterpret_cast<int8 *>(raw);
        *pointer++ = Convert<int8>(x);
        *pointer++ = Convert<int8>(y);
        *pointer++ = Convert<int8>(z);
        *pointer = Convert<int8>(w);
        return *this;
      } else if (config_.layout.elements[i].format == VertexElementFormat::kByte4Normalized) {
        auto pointer = reinterpret_cast<int8 *>(raw);
        *pointer++ = ConvertNormalized<int8>(x);
        *pointer++ = ConvertNormalized<int8>(y);
        *pointer++ = ConvertNormalized<int8>(z);
        *pointer = ConvertNormalized<int8>(w);
        return *this;
      } else if (config_.layout.elements[i].format == VertexElementFormat::kUnsignedByte4) {
        auto pointer = raw;
        *pointer++ = Convert<uint8>(x);
        *pointer++ = Convert<uint8>(y);
        *pointer++ = Convert<uint8>(z);
        *pointer = Convert<uint8>(w);
        return *this;
      } else if (config_.layout.elements[i].format == VertexElementFormat::kUnsignedByte4Normalized) {
        auto pointer = raw;
        *pointer++ = ConvertNormalized<uint8>(x);
        *pointer++ = ConvertNormalized<uint8>(y);
        *pointer++ = ConvertNormalized<uint8>(z);
        *pointer = ConvertNormalized<uint8>(w);
        return *this;
      } else if (config_.layout.elements[i].format == VertexElementFormat::kShort4) {
        auto pointer = reinterpret_cast<int16 *>(raw);
        *pointer++ = Convert<int16>(x);
        *pointer++ = Convert<int16>(y);
        *pointer++ = Convert<int16>(z);
        *pointer = Convert<int16>(w);
        return *this;
      } else if (config_.layout.elements[i].format == VertexElementFormat::kShort4Normalized) {
        auto pointer = reinterpret_cast<int16 *>(raw);
        *pointer++ = ConvertNormalized<int16>(x);
        *pointer++ = ConvertNormalized<int16>(y);
        *pointer++ = ConvertNormalized<int16>(z);
        *pointer = ConvertNormalized<int16>(w);
        return *this;
      } else {
        x_error("unsupported semantic format\n");
      }
    }
  }
  x_error("no semantic in layout\n");
  return *this;
}

MeshUtil &MeshUtil::EndVertex() {
  x_assert(vertex_data_ != nullptr);
  config_.vertex_data = vertex_data_;
  vertex_data_.reset();
  return *this;
}

MeshUtil &MeshUtil::BeginIndex() {
  x_assert(index_data_ == nullptr && config_.index_count > 0 && config_.index_type != IndexFormat::kNone);
  index_data_ = Data::Create(config_.index_count * SizeOfIndexFormat(config_.index_type));
  return *this;
}

MeshUtil &MeshUtil::Index16(size_t index, uint16 value) {
  x_assert(index_data_ != nullptr);
  x_assert(config_.index_type == IndexFormat::kUint16);
  x_assert(index < config_.index_count);
  static_cast<uint16 *>(index_data_->buffer())[index] = value;
  return *this;
}

MeshUtil &MeshUtil::Index32(size_t index, uint32 value) {
  x_assert(index_data_ != nullptr);
  x_assert(config_.index_type == IndexFormat::kUint32);
  x_assert(index < config_.index_count);
  static_cast<uint32 *>(index_data_->buffer())[index] = value;
  return *this;
}

MeshUtil &MeshUtil::Triangle16(size_t index, uint16 a, uint16 b, uint16 c) {
  x_assert(index_data_ != nullptr);
  x_assert(config_.index_type == IndexFormat::kUint16);
  x_assert(index + 3 <= config_.index_count);
  static_cast<uint16 *>(index_data_->buffer())[index] = a;
  static_cast<uint16 *>(index_data_->buffer())[index + 1] = b;
  static_cast<uint16 *>(index_data_->buffer())[index + 2] = c;
  return *this;
}

MeshUtil &MeshUtil::Triangle32(size_t index, uint32 a, uint32 b, uint32 c) {
  x_assert(index_data_ != nullptr);
  x_assert(config_.index_type == IndexFormat::kUint32);
  x_assert(index + 3 <= config_.index_count);
  static_cast<uint32 *>(index_data_->buffer())[index] = a;
  static_cast<uint32 *>(index_data_->buffer())[index + 1] = b;
  static_cast<uint32 *>(index_data_->buffer())[index + 2] = c;
  return *this;
}

MeshUtil &MeshUtil::Quad16(size_t index, uint16 a, uint16 b, uint16 c, uint16 d) {
  x_assert(index_data_ != nullptr);
  x_assert(config_.index_type == IndexFormat::kUint16);
  x_assert(index + 6 <= config_.index_count);
  static_cast<uint16 *>(index_data_->buffer())[index] = a;
  static_cast<uint16 *>(index_data_->buffer())[index + 1] = b;
  static_cast<uint16 *>(index_data_->buffer())[index + 2] = c;
  static_cast<uint16 *>(index_data_->buffer())[index + 3] = a;
  static_cast<uint16 *>(index_data_->buffer())[index + 4] = c;
  static_cast<uint16 *>(index_data_->buffer())[index + 5] = d;
  return *this;
}

MeshUtil &MeshUtil::Quad32(size_t index, uint32 a, uint32 b, uint32 c, uint32 d) {
  x_assert(index_data_ != nullptr);
  x_assert(config_.index_type == IndexFormat::kUint32);
  x_assert(index + 6 <= config_.index_count);
  static_cast<uint32 *>(index_data_->buffer())[index] = a;
  static_cast<uint32 *>(index_data_->buffer())[index + 1] = b;
  static_cast<uint32 *>(index_data_->buffer())[index + 2] = c;
  static_cast<uint32 *>(index_data_->buffer())[index + 3] = a;
  static_cast<uint32 *>(index_data_->buffer())[index + 4] = c;
  static_cast<uint32 *>(index_data_->buffer())[index + 5] = d;
  return *this;
}

MeshUtil &MeshUtil::EndIndex() {
  x_assert(index_data_ != nullptr);
  config_.index_data = index_data_;
  index_data_.reset();
  return *this;
}

} // namespace xEngine