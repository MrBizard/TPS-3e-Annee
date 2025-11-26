#version 330 core

uniform vec2 center;
uniform float rotation;

layout (location = 0) in vec2 position;
out vec4 color;
vec3 HSVtoRGB(vec3 hsv)
{
    float H = hsv.x;
    float S = hsv.y;
    float V = hsv.z;

    if (S == 0.0)
        return vec3(V); // achromatique (gris)

    float fH = H / 60.0; // secteur [0, 6)
    int i = int(floor(fH));
    float f = fH - float(i);
    float p = V * (1.0 - S);
    float q = V * (1.0 - S * f);
    float t = V * (1.0 - S * (1.0 - f));

    vec3 rgb;

    if (i == 0)
        rgb = vec3(V, t, p);
    else if (i == 1)
        rgb = vec3(q, V, p);
    else if (i == 2)
        rgb = vec3(p, V, t);
    else if (i == 3)
        rgb = vec3(p, q, V);
    else if (i == 4)
        rgb = vec3(t, p, V);
    else
        rgb = vec3(V, p, q); // i == 5 ou supérieur

    return rgb;
}

void main()
{
	float dx = position.x - center.x; 
	float dy = position.y - center.y;
	gl_Position = vec4(center.x + (dx * cos(rotation) - dy * sin(rotation)), center.y + (dx * sin(rotation) + dy * cos(rotation)), 0.0, 1.0);
	color = vec4(HSVtoRGB(vec3(degrees(mod(rotation, 2 * 3.1415926538)), 1.0, 1.0)), 1.0);
}