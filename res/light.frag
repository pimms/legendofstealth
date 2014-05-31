uniform sampler2D tex;
uniform float lalpha;
uniform vec4 ulcolor;

float length(vec4 v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

void main()
{
	vec4 src = texture2D(tex, gl_TexCoord[0].xy);
	src.a -= (length(src) - length(ulcolor)) * lalpha*3.0;
	gl_FragColor = src;
}
