#version 450
out vec4 FragColor;
uniform vec3 _Resolution;
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
    
    // Remap to [-1, 1]
    uv = uv * 2.0 - 1.0;
    
    // I CANT GET ASPECT RATIO TO WORK AND EVERYTHING IS STRETCHED AND I DONT KNOW WHY AHHHHHHHH
    float aspectRatio = _Resolution.x / _Resolution.y;
    //uv.x *= aspectRatio;
    //uv.y *= _Resolution.y;
    
    // Create background
    vec3 bgColor = mix(skyTopColor, skyBotColor, 1.0 - uv.y+(sin(_Time * sunSpeed)*1.0 - 0.75));
    
    // Create sun
    vec2 sunPos = vec2(0.0, sin(_Time * sunSpeed)*0.75 - 0.75);
    float sun = circleSDF(uv - sunPos, 0.5);
    sun = 1.0 - smoothstep(-0.035, 0.035,sun);
    
    // Create foreground
    float hills = 1.0 - step(abs(sin(1.65*uv.x + 1.95) * 0.3) - 0.85,uv.y);
    
    // Mix shapes
    vec3 bg = mix(bgColor, sunColor, sun);
    vec3 color = mix(bg, hillsColor, hills);
    FragColor = vec4(color, 1.0);
}