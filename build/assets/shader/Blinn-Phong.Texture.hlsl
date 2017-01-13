struct VS_INPUT
{
    float3 aPosition : POSITION;
    float2 aTexcoord0 : TEXCOORD;
    float3 aNormal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 ViewPosition : POSITION;
    float2 Texcoord : TEXCOORD;
    float3 Normal : NORMAL;
};

#define PS_INPUT VS_OUTPUT

struct PS_OUTPUT
{
    float4 Color : SV_Target;
};

float4x4 uModel;
float4x4 uView;
float4x4 uProjection;

Texture2D uTexture;
SamplerState uSampler;

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4x4 MV_matrix = mul(uView, uModel);
    output.Normal = mul((float3x3)transpose(MV_matrix), input.aNormal);

    float4 ViewVector = mul(MV_matrix, float4(input.aPosition, 1.0));
    output.ViewPosition = ViewVector.xyz;
    
    output.Texcoord = input.aTexcoord0;
    output.Position = mul(uProjection, ViewVector);
    
    return output;
}

PS_OUTPUT PS(PS_INPUT input)
{
    PS_OUTPUT output;

    float3 light_color = float3(1.0, 1.0, 1.0);

    float3 light_direction = normalize(float3(-1.0, 0.0, -1.0));

    float3 normal = normalize(input.Normal);

    float3 ambient = 0.1 * light_color;

    float3 diffuse = 0.5 * max(dot(normal, -light_direction), 0.0) * light_color;

    float3 specular = 0.6 * pow(max(dot(normal, normalize((-light_direction) + (-input.ViewPosition))), 0.0), 16) * light_color;

    float3 result = ambient + diffuse + specular;

    output.Color = float4(result, 1.0) * uTexture.Sample(uSampler, input.Texcoord);

    return output;
}
