#shader vertex 
#version 330 core  
 layout(location=0) in vec3 position;
    out vec4 pcolor ;
 void main()  
 {  
  gl_Position = vec4(position.x,position.y,1.0,1.0);  

  if (position.z == 1)
      pcolor = vec4(1.0, 0.1, 0.1, 0.3);
  else
      pcolor = vec4(0, 0.1, 0, 0.3);
 }  ;


 #shader fragment
 #version 330 core
out vec4 FragColor;
in vec4 pcolor;
 void main()  
 {  	 
     //FragColor = vec4(0.1,0.9,0.1,1.0);
     FragColor = pcolor;
 }  ;