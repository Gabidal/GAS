use "gstd.e"
type import()()

import func _glClear@4()()
import func _glClearColor@16()()
import func _glVertex2f@8()()
import func _glBegin@4()()
import func _glEnd@0()()
import func _glFlush@0()()
import func _glColor3f@12()()
import func _glfwInit()()
import func _glfwTerminate()()
import func _glfwCreateWindow()()
import func _glfwWindowShouldClose()()
import func _glfwPollEvents()()
import func _glfwMakeContextCurrent()()
import func _glfwSwapBuffers()()





func glClear(dword Constant)(
	push Constant
	_glClear@4(),
	(return 0)
)

func glClearColor(dword r, dword g, dword b, dword a)
(
	push a
	push b
	push g
	push r
	(_glClearColor@16())
	return 0
)

func glVertex2f(dword x, dword y)(
	push y
	push x
	(_glVertex2f@8())
	return 0
)

func glBegin(dword Mode)(
	push Mode
	(_glBegin@4())
	return 0
)

func glEnd()(
	(_glEnd@0())
	return 0
)

func glFlush()(
	(_glFlush@0())
	return 0
)

func glColor3f(dword r, dword g, dword b)(
	push b
	push g
	push r
	(_glColor3f@12())
	return 0
)