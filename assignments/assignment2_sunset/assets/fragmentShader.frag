#version 450
out vec4 FragColor;
uniform vec2 _Resolution;
uniform float _Time;
uniform vec3 sunColor;
uniform vec3 skyTopColor;
uniform vec3 skyBotColor;
uniform vec3 hillsColor;
uniform float sunSpeed;
in vec2 UV;
float circleSDF (vec2 p, float r) { return length(p)-r; }
void main(){
	// Normalized pixel coordinates (from 0 to 1)
    vec2 uv = UV;
    
    // Remap sunUV to [-1, 1] while regular uv stays [0, 1]
    vec2 sunUV = uv * 2.0 - 1.0;

    // Set resolution
    float aspectRatio = _Resolution.x / _Resolution.y;
    sunUV.x *= aspectRatio; // don't need to do this with uv.y
    
    // Create background
    vec3 bgColor = mix(skyTopColor, skyBotColor, 1.0 - uv.y+(sin(_Time * sunSpeed)*1.0 - 0.75));
    
    // Create sun
    vec2 sunPos = vec2(0.0, sin(_Time * sunSpeed)*0.75 - 0.75);
    float sun = circleSDF(sunUV - sunPos, 0.5);
    sun = 1.0 - smoothstep(-0.035, 0.035,sun); // or smoothstep(0.4, 0.6, sun)
    
    // Create foreground
    float hills = 1.0 - step(abs(sin(1.65*sunUV.x + 1.95) * 0.3) - 0.85,sunUV.y);
    
    // Mix shapes
    vec3 bg = mix(bgColor, sunColor, sun);
    vec3 color = mix(bg, hillsColor, hills);
    FragColor = vec4(color, 1.0);
}