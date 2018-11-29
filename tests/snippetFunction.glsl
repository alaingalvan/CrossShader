// 💡 Brightness / Contrast
vec4 brightnessContrast(vec4 inColor, float brightness, float contrast)
{
  return vec4((inColor.rgb - 0.5) * contrast + 0.5 + brightness, inColor.a);
}