
#include "Angel.h"
#include <string.h>

typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)

point4 points[NumVertices];
color4 colors[NumVertices];
point4 quad_data[342];
point4 strip_data[40];
// color4 spherecolors[40];

point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 0.5, 0., 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};


// Parameters controlling the size of the Robot's arm
const GLfloat BASE_HEIGHT      = 2.0;
const GLfloat BASE_WIDTH       = 5.0;
const GLfloat LOWER_ARM_HEIGHT = 5.0;
const GLfloat LOWER_ARM_WIDTH  = 0.5;
const GLfloat UPPER_ARM_HEIGHT = 5.0;
const GLfloat UPPER_ARM_WIDTH  = 0.5;

// Shader transformation matrices
mat4  model_view;
GLuint ModelView, Projection;

// Array of rotation angles (in degrees) for each rotation axis
enum { Base = 0, LowerArm = 1, UpperArm = 2, NumAngles = 3 };
int      Axis = Base;
GLfloat  Theta[NumAngles] = { 0.0 };

// Menu option values
const int  Quit = 4;

//world to view matrix
mat4 view = mat4(1);


//等腰三角形底角角度
float theta = 0;
float base_degree= 0;
float lower_arm_degree = 0;
float upper_arm_degree = 0;

float base_degree2= 0;
float lower_arm_degree2 = 0;
float upper_arm_degree2 = 0;



mat4 topView = LookAt(vec4(0,0,0,1),vec4(0,-1,0,1), vec4(0,0,-1,1)); // top view matrix, arguments: eye, at, up and this results in the top view that's located on the bottom right corner
                                                                        // because the matrix generated is actually a matrix apply to the world coordinate system that moves the world
                                                                        //down and flip up towards the camera, this is why it'll be at the right bottom corner??? maybe, not sure

//input argument
point4 old_position;
point4 new_position; 
float ball_radius = 0.25;
// float x_value = old_position.x - ball_radius;


int i = 0;
//----------------------------------------------------------------------------

int Index = 0;

void
quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[d]; Index++;
}

void 
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

void 
colorsphere()
{
    const float DegreesToRadians = M_PI / 180.0; // M_PI = 3.14159...

    // 8 rows of 18 quads
    int k = 0;
   float r = 0.25;
    for(float phi = -80.0; phi <= 80.0; phi += 20.0)
    {
        float phir = phi*DegreesToRadians;
        float phir20 = (phi + 20.0)*DegreesToRadians;
        for(float theta = -180.0; theta <= 180.0; theta += 20.0)
        {
            float thetar = theta*DegreesToRadians;
            quad_data[k] = point4(r* sin(thetar)*cos(phir),
                                    r * cos(thetar)*cos(phir), r * sin(phir), 1.0);
            k++;
            quad_data[k] = point4(r* sin(thetar)*cos(phir20),
                                    r * cos(thetar)*cos(phir20), r * sin(phir20), 1.0);
            k++;
        }
    }

    
    // k = 0;
    // point4 strip_data[40];

    // strip_data[k] = point4(0.0, 0.0, 1.0, 1.0);
    
    // k++;

    // float sin80 = sin(80.0*DegreesToRadians);
    // float cos80 = cos(80.0*DegreesToRadians);

    // for(float theta = -180.0; theta <= 180.0; theta += 20.0)
    // {
    //  float thetar = theta*DegreesToRadians;
    //  strip_data[k] = point4(sin(thetar)*cos80,
    //                          cos(thetar)*cos80, sin80, 1.0);
        
    //  k++;
    // }

    // strip_data[k] = point4(0.0, 0.0, -1.0, 1.0);
    
    // k++;

    // for(float theta = -180.0; theta <= 180.0; theta += 20.0)
    // {
    //  float thetar = theta;
    //  strip_data[k] = point4(sin(thetar)*cos80,
    //                          cos(thetar)*cos80, sin80, 1.0);
        
    //  k++;
    // }
}


//----------------------------------------------------------------------------

/* Define the three parts */
/* Note use of push/pop to return modelview matrix
to its state before functions were entered and use
rotation, translation, and scaling to create instances
of symbols (cube and cylinder */

void
base()
{
    mat4 instance = ( Translate( 0.0, 0.5 * BASE_HEIGHT, 0.0) *
         Scale( BASE_WIDTH,
            BASE_HEIGHT,
            BASE_WIDTH ) );

    //glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view * instance );
    glUniformMatrix4fv( ModelView, 1, GL_TRUE, view * model_view * instance );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------

void
upper_arm()
{
    mat4 instance = ( Translate( 0.0, 0.5 * UPPER_ARM_HEIGHT, 0.0) *
              Scale( UPPER_ARM_WIDTH,
                 UPPER_ARM_HEIGHT,
                 UPPER_ARM_WIDTH ) );
    
    //glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view * instance );
    glUniformMatrix4fv( ModelView, 1, GL_TRUE, view * model_view * instance );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );

}

//----------------------------------------------------------------------------

void
lower_arm()
{
    mat4 instance = ( Translate( 0.0, 0.5 * LOWER_ARM_HEIGHT, 0.0 ) *
              Scale( LOWER_ARM_WIDTH,
                 LOWER_ARM_HEIGHT,
                 LOWER_ARM_WIDTH ) );
    
    //glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view * instance );
    glUniformMatrix4fv( ModelView, 1, GL_TRUE, view *  model_view * instance );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices ); 
}

//----------------------------------------------------------------------------

void 
sphere()
{
    mat4 instance =  Translate( old_position[0], old_position[1], old_position[2] ) ;
    if (i < 11)
    {
        glUniformMatrix4fv( ModelView, 1, GL_TRUE, view * instance );
    }
    else
    {
        glUniformMatrix4fv( ModelView, 1, GL_TRUE, view * model_view );
    }


    glDrawArrays( GL_LINE_LOOP, 80, 342);
}

//----------------------------------------------------------------------------

// void
// poles()
// {
//  mat4 instance = ( Translate( 0.0, 13, 0.0 ) );
//  glUniformMatrix4fv( ModelView, 1, GL_TRUE, instance );
//  //glDrawArrays( GL_TRIANGLE_FAN, 0, 40 );
// }

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // glutWireSphere(0.25, 50, 50);
    // glutWireTeapot(1);
    // Accumulate!!!!!!! ModelView Matrix as we traverse the tree
    model_view = RotateY(Theta[Base]);
    base();

    model_view *= ( Translate(0.0, BASE_HEIGHT, 0.0) *
            RotateZ(Theta[LowerArm] ) );
    lower_arm();

    model_view *= ( Translate(0.0, LOWER_ARM_HEIGHT, 0.0) *
            RotateZ(Theta[UpperArm]) );
    upper_arm();

    model_view *= Translate(0.0, UPPER_ARM_HEIGHT + ball_radius, 0.0);
    sphere();

    // poles();   
    // glutWireTeapot(1);
    
    glutSwapBuffers();
}

void
reshape( int width, int height )
{
    glViewport( 0, 0, width, height );

    GLfloat  left = -10.0, right = 10.0;
    GLfloat  bottom = -5.0, top = 15.0;
    GLfloat  zNear = -10.0, zFar = 10.0;

    GLfloat aspect = GLfloat(width)/height;

    if ( aspect > 1.0 ) {
    left *= aspect;
    right *= aspect;
    }
    else {
    bottom /= aspect;
    top /= aspect;
    }

    mat4 projection = Ortho( left, right, bottom, top, zNear, zFar ); // this projection matrix projects every point to the negative z side
    glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );

    model_view = mat4( 1.0 );  // An Identity matrix
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
    case 033: // Escape Key
    case 'q': case 'Q':
        exit( EXIT_SUCCESS );
        break;

    case '1':
        Axis = Base;
        break;
    case '2':
        Axis = LowerArm;
        break;
    case '3':
        Axis = UpperArm;
        break;
    }
}

void
mouse( int button, int state, int x, int y )
{

    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) {
    // Incrase the joint angle
    Theta[Axis] += 45.0;
    if ( Theta[Axis] > 360.0 ) { Theta[Axis] -= 360.0; }
    }

    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN ) {
    // Decrase the joint angle
    Theta[Axis] -= 45.0;
    if ( Theta[Axis] < 0.0 ) { Theta[Axis] += 360.0; }
    }

    glutPostRedisplay();
}


//----------------------------------------------------------------------------

void
init( void )
{
    colorcube();
    colorsphere();

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(quad_data) + sizeof(strip_data), NULL, GL_DYNAMIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors), sizeof(quad_data), quad_data );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(quad_data), sizeof(strip_data), strip_data );
    // glBufferSubData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors) + sizeof(strip_data), sizeof(spherecolors), spherecolors);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader81.glsl", "fshader81.glsl" );
    glUseProgram( program );
    
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
               BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
               BUFFER_OFFSET(sizeof(points)) );

    ModelView = glGetUniformLocation( program, "ModelView" );
    Projection = glGetUniformLocation( program, "Projection" );

    glEnable( GL_DEPTH );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); 
}


//----------------------------------------------------------------------------


void printPositions()
{
    printf("old position is x:%f, y:%f, z:%f\n", old_position[0], old_position[1], old_position[2]);
    printf("new position is x:%f, y:%f, z:%f\n", new_position[0], new_position[1], new_position[2]);
    
}

float lowerArmRotation(float opposite, float hypotenuse )
{
    float randians1 = asin( opposite / hypotenuse ); 
    float beta = randians1  * 180 / M_PI; //compute angle between the ball and the lower end of the lower arm
    float randians2 = acos( hypotenuse/2 /5 );  
    theta = randians2 * 180 / M_PI; //compute 等腰三角形底角角度
    return beta - theta;
}

float baseRotation(float adjacent)
{
    float randians = atan( old_position.z / adjacent ); //calculate rotation in randians
    float angle = randians  * 180 / M_PI; //convert to degree
    return angle;
}

void rotationDelay(int)
{
    if (i < 10)
    {
        Theta[LowerArm] -= lower_arm_degree/10;
        Theta[UpperArm] -= upper_arm_degree/10;
        Theta[Base] -= base_degree/10;
        i++;
        glutPostRedisplay();
        glutTimerFunc(500.0, rotationDelay, 0);    
    }
    else if (i == 10)
    {
        Theta[LowerArm] += lower_arm_degree;
        Theta[UpperArm] += upper_arm_degree;
        Theta[Base] += base_degree;
        i++;
        glutPostRedisplay();
        glutTimerFunc(500.0, rotationDelay, 0); 
    }
    else if (i > 10 && i < 21)
    {
        Theta[LowerArm] -= lower_arm_degree2/10;
        Theta[UpperArm] -= upper_arm_degree2/10;
        Theta[Base] -= base_degree2/10;
        i++;
        glutPostRedisplay();
        glutTimerFunc(500.0, rotationDelay, 0); 
    }
}

void computeRotation()
{
    float x_value = old_position.x - ball_radius;
    float distance = sqrt( x_value* x_value + old_position.z *old_position.z );
    float bottom = sqrt( x_value* x_value + (old_position.y - 2) * (old_position.y - 2) + old_position.z *old_position.z ); //find the length of the bottom side of the 等腰三角形
    base_degree = baseRotation(x_value);
    lower_arm_degree = lowerArmRotation( distance, bottom);
    upper_arm_degree = 2 * theta;  
    if (x_value < 0)
    {
         base_degree  = -base_degree;
         lower_arm_degree = -lower_arm_degree;
         upper_arm_degree = -upper_arm_degree;
    }
    printf("old: base_degree is %f, upper_arm_degree is %f, lower_arm_degree is %f\n", base_degree, upper_arm_degree, lower_arm_degree);

    //compute degree to place the ball in new position
    float x_value2 = new_position.x - ball_radius;
    float distance2 = sqrt( x_value2* x_value2 + new_position.z *new_position.z );
    float bottom2 = sqrt( x_value2* x_value2 + (new_position.y - 2) * (new_position.y - 2) + new_position.z *new_position.z ); //find the length of the bottom side of the 等腰三角形
    base_degree2 = baseRotation(x_value2);
    lower_arm_degree2 = lowerArmRotation( distance2, bottom2);
    upper_arm_degree2 = 2 * theta;
    if (x_value2 < 0)
    {
         base_degree2  = -base_degree2;
         lower_arm_degree2 = -lower_arm_degree2;
         upper_arm_degree2 = -upper_arm_degree2;
    }
    printf("new: base_degree is %f, upper_arm_degree is %f, lower_arm_degree is %f\n", base_degree2, upper_arm_degree2, lower_arm_degree2);

}

//----------------------------------------------------------------------------

int main( int argc, char **argv )
{
    if (argc == 8) {
        old_position = point4(atof(argv[1]), atof(argv[2]), atof(argv[3]), 1.0);
        new_position = point4(atof(argv[4]), atof(argv[5]), atof(argv[6]), 1.0);

        if (strcmp(argv[7], "-tv") == 0) {
            printf("topview is true\n");
            view *= topView;
        }
    }

    computeRotation();

    //compute degree to grab the ball

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "robot" );

    // Iff you get a segmentation error at line 34, please uncomment the line below
    glewExperimental = GL_TRUE; 
    glewInit();
    
    init();

    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutTimerFunc(500.0, rotationDelay, 0); 

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // glEnable(GL_DEPTH_TEST);
    // glDepthFunc(GL_LESS);

    glutMainLoop();

    return 0;
}
