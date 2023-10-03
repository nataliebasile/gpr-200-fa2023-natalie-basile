#version 450
out vec4 FragColor;
in vec2 UV;

uniform vec2 _Resolution;
uniform float _Time;
uniform sampler2D _BrickTexture;
uniform sampler2D _PuddleSlimeTexture;
uniform sampler2D _NoiseTexture;

void main(){
	vec2 uv = UV;
	vec2 uvBrick = UV;
	vec2 uvSlime = UV;

	vec4 noiseColor = texture(_NoiseTexture, uv);
	float noise = texture(_NoiseTexture, uv + _Time * 0.5).r;
	
	uvSlime = uv * (sin(_Time + 1.0)) + noise * 0.1f;
	uvBrick.x = UV.x + _Time * 0.25;

	vec4 brickColor = texture(_BrickTexture, uvBrick);
	vec4 slimeColor = texture(_PuddleSlimeTexture, uvSlime);

	vec3 color = mix(brickColor.rgb, slimeColor.rgb, slimeColor.a);
	FragColor = vec4(color, 1.0);

}