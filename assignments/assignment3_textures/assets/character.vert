#version 450
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 vUV;
out vec2 UV;
uniform float _Time;
void main(){
	UV = vUV;
	vec3 vPosSlime = vPos;
	vPosSlime.y = vPos.y + sin(_Time);
	gl_Position = vec4(vPosSlime/2.0,1.0);
}