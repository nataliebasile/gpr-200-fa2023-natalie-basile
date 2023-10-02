#version 450
out vec4 FragColor;
in vec2 UV;

uniform vec2 _Resolution;
uniform float _Time;
uniform sampler2D _Texture;
//uniform sampler2D _BrickTexture;
//uniform sampler2D _PuddleSlimeTexture;
//uniform sampler2D _NoiseTexture;

void main(){
	vec2 uv = UV;
	float aspectRatio = _Resolution.x / _Resolution.y;
	//uv.x *= aspectRatio;

	//mix(_BrickTexture, _PuddleSlimeTexture, uv.y);

	FragColor = texture(_Texture, uv);

}