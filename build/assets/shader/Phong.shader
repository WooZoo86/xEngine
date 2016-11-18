//////////////////////////////////////////////////////

@begin vertex

<-in-> <-float3-> aPosition;
<-in-> <-float2-> aTexcoord0;
<-in-> <-float3-> aNormal;

<-out-> <-float2-> Texcoord;
<-out-> <-float3-> Normal;

<-uniform-> <-float4x4-> uModel;
<-uniform-> <-float4x4-> uView;
<-uniform-> <-float4x4-> uProjection;

<-vs-> {
  Texcoord = aTexcoord0;
  Normal = {- mul || <-float3x3->(transpose(inverse(uModel))) || aNormal -};
  <-float4-> Model = {- mul || uModel || <-float4->(aPosition, 1.0) -};
  <-float4-> View = {- mul || uView || Model -};
  <-vs_position-> = {- mul || uProjection || View -};
}

@end vertex

//////////////////////////////////////////////////////

@begin fragment

<-in-> <-float2-> Texcoord;
<-in-> <-float3-> Normal;

<-out-> <-float4-> <-fs_color->;

<-uniform-> <-sampler2d-> uTexture;

<-fs-> {
  <-float3-> light_color = <-float3->(1.0, 1.0, 1.0);
  <-float3-> light_direction = normalize(<-float3->(1.0, 0.0, 1.0));

  <-float3-> normal = normalize(Normal);

  <-float-> ambient_strength = 0.1;
  <-float3-> ambient = ambient_strength * light_color;

  <-float-> diffuse_strength = max(dot(normal, light_direction), 0.0);
  <-float3-> diffuse = diffuse_strength * light_color;

  <-float-> specular_strength = 0.5;
  <-float3-> reflect_direction = reflect(-light_direction, normal);
  <-float-> specular_pow = pow(max(dot(light_direction, reflect_direction), 0.0), 32);
  <-float3-> specular = specular_strength * specular_pow * light_color;

  <-float3-> result = ambient + diffuse + specular;

  <-fs_color-> = <-float4->(result, 1.0) * {- sample || uTexture || Texcoord -};
}

@end fragment

//////////////////////////////////////////////////////
