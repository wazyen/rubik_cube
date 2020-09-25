
varying vec3 v_position;
varying vec3 v_world_position;
varying vec3 v_normal;
varying vec3 v_world_normal;
varying vec2 v_uv;
varying vec4 v_color;

uniform vec3 u_eye;

uniform bool u_white;
uniform bool u_red;
uniform bool u_blue;
uniform bool u_yellow;
uniform bool u_orange;
uniform bool u_green;

void main()
{
	vec2 uv = v_uv;
	
	vec3 world_normal = v_world_normal;
	vec3 color;

	// The world normal of each face defines the color of that face
	// There are six possible normals (otherwise there would be some mistake), one for each rubik piece face:
	// However, we have to take into account that there may be some black faces
	// (1,0,0): blue
	// (-1,0,0): green
	// (0,1,0): white
	// (0,-1,0): yellow
	// (0,0,1): red
	// (0,0,-1): orange

	// We pre-store the vector of each color inside a variable for convenience and legibility later on
	// Color RGB codes extracted from https://www.schemecolor.com/rubik-cube-colors.php
	vec3 black = vec3(0.2,0.2,0.2);
	vec3 white = vec3(1,1,1);
	vec3 red = vec3(0.73,0,0);
	vec3 blue = vec3(0,0.27,0.68);
	vec3 yellow = vec3(1,0.84,0);
	vec3 orange = vec3(1,0.35,0);
	vec3 green = vec3(0,0.61,0.28);

	// we want to know whether the world normal vector has a 1 or a -1 in any position
	// if it's positive, the face color can only be blue, white or red; otherwise green, yellow or orange
	// there will always be only one component being 1 or -1 and the rest being 0
	// consequently, if we sum the product of each component with its corresponding color, we will get at the end the color of the face
	// in the case that the component is -1, we have to subtract instead of summing
	// we also have to take into account if the piece that we are rendering actually contains that color; all of them only have 1, 2 or 3 colored faces and the rest are black
	float aux = world_normal.x + world_normal.y + world_normal.z;
	if(aux > 0.0)
	{
		color = world_normal.x * (u_blue ? blue : black) + world_normal.y * (u_white ? white : black) + world_normal.z * (u_red ? red : black);
	}
	else
	{
		color = -world_normal.x * (u_green ? green : black) - world_normal.y * (u_yellow ? yellow : black) - world_normal.z * (u_orange ? orange : black);
	}
	
	vec3 P = v_world_position;
	vec3 eye = u_eye;
	vec3 light = vec3(-300.f,300.f,-300.f);
	
	vec3 Ia = vec3(0.3,0.3,0.3);
	vec3 Id = vec3(1.0,1.0,1.0);
	vec3 Is = vec3(1,1,1);
	vec3 K = color;
	float shininess = 10.f;
	
	vec3 L = normalize( light - P );
	vec3 N = normalize( v_normal );
	vec3 R = normalize( reflect( -L, N ) );
	vec3 V = normalize( eye - P );

	vec3 color_a = K * Ia;
	vec3 color_d = K * Id * max( 0, dot( L, N ) );
	vec3 color_s = K * Is * pow( max( dot( R, V ), 0 ), shininess );
	vec3 color_p = color_a + color_d + color_s;

	gl_FragColor = vec4(color_p,1.0);
}