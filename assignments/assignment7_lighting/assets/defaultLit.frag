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

struct Light {
	vec3 position;
	vec3 color;
};

uniform Material _Material;
uniform Light _Light;
uniform vec3 _CameraPos;
uniform sampler2D _Texture;

void main(){
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 ambient, diffuse, specular, color;
	vec3 w = normalize(_Light.position - fs_in.WorldPos);
	vec3 v = normalize(_CameraPos - fs_in.WorldPos);
	vec3 h = normalize(w + v);

	ambient = _Light.color * _Material.ambientK;
	diffuse = _Light.color * _Material.diffuseK * max(dot(w, normal), 0);
	specular = _Light.color * _Material.specular * pow(max(dot(h, normal), 0), _Material.shininess);

	color =  ambient + diffuse + specular;

	FragColor = vec4(color, 1.0) * texture(_Texture, fs_in.UV);
}