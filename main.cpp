#include <GL/freeglut.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <fstream>

std::vector<std::string> lines = {""};
int currentLine = 0;
int currentChar;
std::ofstream myFile;
std::string filename;



// this kinda works, but you want a set of widths for each letter in the font
float bitmapLength(std::string string) {

    float pixelLength = 21.333 * (string.size() + 1);
    float percentLength = pixelLength / 500;
    float endLocation = -0.55f + percentLength;
    return endLocation;
}


void loadFile() {
    std::string tempString;
    std::ifstream tempFile (filename);
    if (tempFile.is_open()) {
        char tempChar;
        while (tempFile) {
            tempChar = tempFile.get();
            if (tempChar == '\n') {
                lines.push_back("");
                currentLine++;
            } else {
                lines[currentLine] += tempChar;
            }
        }
        lines[currentLine].pop_back();
        currentChar = lines[currentLine].size();
    } else {
        system("touch filename");
        currentChar = 0;
        lines = {""};
    }
}

void renderLines() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.f, 0.f, 0.f);

   for (int i = 0; i < lines.size(); ++ i) {

        glRasterPos2f(-0.5f, 0.5f - (i * 0.1));
        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned const char*)lines[i].c_str());

   }

    // draw line indicator
    std::string cursor = "|";
    glColor3f(0.f, 1.f, 0.f);
    glRasterPos2f(bitmapLength(lines[currentLine]), .5f - (currentLine * 0.1));
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (unsigned const char*)cursor.c_str());



    glutSwapBuffers();

}

void save() {
    myFile.open(filename);
    for (std::string line : lines) {
        myFile << line << "\n";
    }
    myFile.close();
}


// 13 enter
// 8 backspace
void keyPress(unsigned char key, int x, int y) {

    int mod = glutGetModifiers();

    if (mod = GLUT_ACTIVE_CTRL) {
        // std::cout << "check " << (int)key << std::endl;
    }

    switch((int)key)
    {
        
        case 13:
            currentLine++;
            lines.push_back("");
            break;
        
        case 8:
            if (currentLine == 0 && lines[0] == "") break;
            if (lines[currentLine] == "") {
                lines.erase(lines.begin() + currentLine);
                currentLine--;
                currentChar = lines[currentLine].size();
            } else {
                lines[currentLine].pop_back();
                currentChar--;
            }
            break;

        case 19:
            save();
            break;
    
        case 23:
            exit(0);
            break;

        default:
            if (glutGetModifiers() == GLUT_ACTIVE_CTRL) break;
            // std::cout << (int)key << std::endl;
            lines[currentLine] += key;
            currentChar++;
            break;
    }
renderLines();
}

int main(int argc, char *argv[]) {

    filename = argv[1];
    loadFile();


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Window");
    glutKeyboardFunc(keyPress);

    glutDisplayFunc(renderLines);

    glutMainLoop();
}