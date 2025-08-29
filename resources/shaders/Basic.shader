#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
uniform float offsetY;
uniform float offsetX;

void main()
{
    gl_Position = vec4(aPos.x + offsetX, aPos.y + offsetY, aPos.z, 1.0);
};

#shader fragment
#version 330 core
        
out vec4 color;
uniform vec4 cor; 

void main()
{
    color = cor; 
};
