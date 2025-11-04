// Shader-ul de fragment / Fragment shader 
#version 330 core

in vec4 ex_Color;
out vec4 out_Color;

in vec2 tex_Coord;


uniform sampler2D myTexture;
uniform int withTexture;
uniform int color;

void main(void)
  {
    //float debugValue = tex_Coord.x * tex_Coord.y;
    //out_Color = vec4(debugValue, 0.0, 0.0, 1.0);
    if (withTexture == 1)
    {
        out_Color = texture(myTexture, tex_Coord);
    }
    else
    {
        out_Color = vec4(0.6f, 0.3f, 0.0f, 1.0f);
    }
    if(color == 2)
    {
            out_Color = vec4(1.0, 1.0,1.0, 1.0);
    }
  }