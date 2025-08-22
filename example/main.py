import moderngl
import glfw
import numpy as np

if not glfw.init():
    raise Exception("failed to init glfw")

window = glfw.create_window(800, 600, "try-opengl", None, None)
if not window:
    glfw.terminate()
    raise Exception("failed to create window")

glfw.make_context_current(window)
ctx = moderngl.create_context()
ctx.viewport = (0, 0, 800, 600)

def framebuffer_size_callback(window, width, height):
    ctx.viewport = (0, 0, width, height)
glfw.set_framebuffer_size_callback(window, framebuffer_size_callback)

def process_input(window):
    if glfw.get_key(window, glfw.KEY_ESCAPE) == glfw.PRESS:
        glfw.set_window_should_close(window, True)
    
vertices = np.array([
    -0.5, -0.5, 0.0,
     0.5, -0.5, 0.0,
     0.0,  0.5, 0.0
], dtype='f4')
vbo = ctx.buffer(vertices.tobytes(), dynamic=True)

vertex_shader = """
#version 330 core

in vec3 in_vert;

void main() {
    gl_Position = vec4(in_vert.x, in_vert.y, in_vert.z, 1.0);
}
"""

fragment_shader = """
#version 330 core

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
"""

prog = ctx.program(vertex_shader, fragment_shader)
vao = ctx.vertex_array(prog, vbo, 'in_vert')

while not glfw.window_should_close(window):
    process_input(window)

    ctx.clear(0.2, 0.3, 0.3)
    vao.render(moderngl.TRIANGLES)

    glfw.poll_events()
    glfw.swap_buffers(window)

glfw.terminate()