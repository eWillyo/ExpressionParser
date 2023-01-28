# Expression parser


 Simple math solver with variables, goniometric functions, logarithms, vectors, etc. This project should suits C++11 standards. Original idea dates back to 2010. I significantly inspired myself on "codeproject.com", when I was creating this easy to implement expression parser. After years I came back to project and finally try to finish it. 
 
 
  [![Expression input](https://github.com/eWillyo/ExpressionParser/blob/a966ae5f640ba9ece3d49580170910264f50ac47/expression_input.png?raw=true)](https://www.youtube.com/watch?v=lFQNcjrtSyo)


# Dependencies


The only library which this project depends on (except STL) is GLM [https://github.com/g-truc/glm]. GLM is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.


 # NEW: 
 
 
  * Fixed few bugs and added support for basic vector operations
  * Added basic support for matrices
  * Added few new operations (modulo, factorial, ...)
  * Better result formatting (Eg. "9876543211.09879" -> "9,876,543,211.098789")
  * Added rand() function (range: 0..999)
  * Better error handling
  * Added hyperbolic functions; matrix casting
  * Added matrix-scalar operations
 
 
 # TO DO: 
 
 
  * Support for loading scripts from file
  * Binary, hexadecimal, octal to decimal conversion
  * Add arrays
  * Add vector casting
  * Custom defined functions
  * Optimize code
 
 
 # Examples:
 
          Enter expression: 9 / 8
          Result: 1.125

          Enter expression: cos(pi)
          Result: -1
          
          Enter expression: a = 6.66
          Enter expression: 3 + (a - 7)^4
          Result: 3.01336
          
          Enter expression: abs(log(1/1000))
          Result: 3
          
          Enter expression: 113 % 10
          Result: 3
          
          Enter expression: 9! / 8!
          Result: 9
          
          Enter expression: v1 = vec3(1,0,0)
          Enter expression: v2 = vec3(0,2,0)
          Enter expression: v2 = normalize(v2)
          Enter expression: DEG(acos(dot(v1,v2)))
          Result: 90
          
          Enter expression: t = rotate(translate(mat4(), vec3(10,10,10)), RAD(rand(360)), vec3(0,1,0))
          Enter expression: t * vec4(6,6,6,1)
          Result: ( 2.883644, 16, 14.621415, 1 )
          


NOTE: To install on Linux (Debian) just copy "create_solver.sh" file and run it with these commands:

         wget https://raw.githubusercontent.com/eWillyo/ExpressionParser/main/create_solver.sh
         chmod +x create_solver.sh
         sudo ./create_solver.sh

