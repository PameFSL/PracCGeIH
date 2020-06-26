#version 330
layout (location =0) in vec3 pos; 
out vec4 vColor;
uniform float c1;
uniform float c2;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	gl_Position=projection* view* model * vec4(pos,1.0f);
	if (c2==0){
		vColor=vec4(clamp(pos,0.0f,1.0f),1.0f);
	}
	else{
		vColor.x=c1;
		vColor.z=c2;
	}
}