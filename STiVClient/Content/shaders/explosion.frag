#version 120

uniform int targetsCount;
uniform vec2 targetPos[100], textureSize;
uniform float targetTime[100];
uniform sampler2D currentTexture;

vec4 colorFromPx(vec2 pxFrom)
{
	pxFrom /= textureSize;
	pxFrom.y = 1. - pxFrom.y;
	return texture2D(currentTexture, pxFrom);
}

void main()
{
	vec2 procPos = gl_TexCoord[0].st;
	procPos.y = 1. - procPos.y;
	procPos *= textureSize;
	
	vec2 pxFrom = procPos;
	vec4 outputColor = texture2D(currentTexture, gl_TexCoord[0].st);
	
	for (int i = 0; i < targetsCount; ++i)
	{
		vec2 relat = procPos - targetPos[i];
		float d = length(relat);
		float phi = atan(relat.y, relat.x);
		
		if (d < 15.)
		{
			float r = d - (targetTime[i]/100) * sqrt(d);
			
			pxFrom.x = targetPos[i].x + r * cos(phi);
			pxFrom.y = targetPos[i].y + r * sin(phi);
			outputColor = colorFromPx(pxFrom);
		}
		if (d < 40. && d >= 10. && d < targetTime[i]/3.)
		{
			pxFrom.x = targetPos[i].x + sqrt(d) * cos(phi);
			pxFrom.y = targetPos[i].y + sqrt(d) * sin(phi);
			outputColor = mix(colorFromPx(pxFrom), outputColor, 0.0414835*d - 0.000412088*d*d);
		}
	}
	
	gl_FragColor = outputColor;
}