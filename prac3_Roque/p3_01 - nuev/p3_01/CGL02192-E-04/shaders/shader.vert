#version 330
layout (location =0) in vec3 pos; 
out vec4 vColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 n_color;
uniform float c2;
void main()
{
	gl_Position=projection* view* model * vec4(pos,1.0f);
	vColor=vec4(0.3,n_color.y+0.3f,1.0+0.2,1.0f);
}