#version 330 core
out vec4 FragColor;

uniform vec4 color; // The base color of the cube
uniform float opacity; // Opacity value ranging from 0.0 to 1.0

void main()
{
    // Set the fragment color by multiplying the base color with opacity
    FragColor = vec4(color.rgb, color.a * opacity); // Ensure RGB values are used with the specified opacity
}
