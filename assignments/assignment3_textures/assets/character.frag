#version 450
out vec4 FragColor;
in vec2 UV;

uniform vec2 _Resolution;
uniform float _Time;
uniform sampler2D _CharacterTexture;

void main(){	
	vec2 uv = UV;

	vec4 characterColor = texture(_CharacterTexture, uv);

	//vec3 color = mix(brickColor.rgb, slimeColor.rgb, slimeColor.a);

	FragColor = vec4(characterColor);

}