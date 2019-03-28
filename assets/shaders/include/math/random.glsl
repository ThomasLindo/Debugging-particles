float rand(float co)
{
	return fract(sin(1000.0*dot(vec2(co), vec2(21.5739,43.421))) * 617284.3);
}

float rand(vec2 co)
{
	return fract(sin(1000.0*dot(co.xy, vec2(21.5739,43.421))) * 617284.3);
}