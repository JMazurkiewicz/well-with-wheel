#version 330 core

in vec3 PositionWorld;
in vec3 NormalC;
in vec3 LightDirCamera;
in vec2 UV;

out vec3 color;

uniform mat4 MVP;
uniform mat4 V;
uniform mat4 M;
uniform sampler2D TextureSampler;
uniform vec3 LightPosWorld;

void main() {
	vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);
	float LightPower = 10.0f;

	vec3 MaterialDiffuseColor = texture(TextureSampler, UV).rgb;
	vec3 MaterialAmbientColor = vec3(0.1f, 0.1f, 0.1f) * MaterialDiffuseColor;

	float distance = length(LightPosWorld - PositionWorld);

	vec3 n = normalize(NormalC);
	vec3 l = normalize(LightDirCamera);
	float cosTheta = clamp(dot(n, l), 0.0f, 1.0f);

    color =
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance * distance)
		+ MaterialAmbientColor;
}
