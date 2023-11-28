#version 450
out vec4 FragColor;

in Surface{
	vec2 UV; // per-fragment interpolated UV
	vec3 WorldPos; // per-fragment interpolated world position
	vec3 WorldNormal; // per-fragment interpolated world normal
}fs_in;

struct Material {
	float ambientK; // Ambient coefficient (0-1)
	float diffuseK; // Diffuse coefficient (0-1)
	float specular; // Specular coefficient (0-1)
	float shininess; // Shininess
};
uniform Material _Material;

struct Light {
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Lights[MAX_LIGHTS];

uniform vec3 _CameraPos;
uniform sampler2D _Texture;

void main(){
	vec3 totalColor;
	for (int i = 0; i < MAX_LIGHTS; i++) {
		vec3 normal = normalize(fs_in.WorldNormal);
		vec3 ambient, diffuse, specular, color;
		vec3 w = normalize(_Lights[i].position - fs_in.WorldPos);
		vec3 v = normalize(_CameraPos - fs_in.WorldPos);
		vec3 h = normalize(w + v);

		ambient = _Lights[i].color * _Material.ambientK;
		diffuse = _Lights[i].color * _Material.diffuseK * max(dot(w, normal), 0);
		specular = _Lights[i].color * _Material.specular * pow(max(dot(h, normal), 0), _Material.shininess);

		color +=  ambient + diffuse + specular;
		totalColor += color;
	}

	FragColor = vec4(totalColor, 1.0) * texture(_Texture, fs_in.UV);
}