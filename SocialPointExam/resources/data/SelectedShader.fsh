#ifdef GL_ES
precision mediump float;
#endif
uniform vec3 u_borderColor;
uniform float u_borderAlpha;
uniform vec3 u_color;
uniform vec2 u_blurResolution;
varying vec2 v_texCoord;
uniform sampler2D CC_Texture0;

void main()
{
    gl_FragColor = texture2D(CC_Texture0, v_texCoord);
    
    //If somewhere between complete transparent and completely opaque
    float alpha = texture2D(CC_Texture0, v_texCoord).a;
    if (alpha < 0.5 && alpha > 0.2)
    {
            gl_FragColor.rgb += u_borderColor*u_borderAlpha;
    }
}