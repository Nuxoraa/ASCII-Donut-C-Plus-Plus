#include <iostream>    // standard input-output stream for printing characters
#include <vector>      // dynamic array container to store the frame buffers
#include <cmath>       // math library for sine and cosine functions
#include <thread>      // library for thread handling, used for the sleep function
#include <chrono>      // time library to define millisecond durations

#if defined(_WIN32)    // check if the operating system is windows
    #include <windows.h> // include windows-specific api for console handling
#else                  // if the system is linux or macos
    #include <sys/ioctl.h> // include ioctl for terminal terminal control
    #include <unistd.h>    // standard symbolic constants and types
#endif

int main() {
    float a = 0;       // angle of rotation around the x-axis (pitch)
    float b = 0;       // angle of rotation around the z-axis (yaw)

    std::cout << "\x1b[?25l"; // ansi escape code to hide the terminal cursor

    while (true) {     // start of the infinite animation loop
        int term_w = 80;  // default terminal width if detection fails
        int term_h = 24;  // default terminal height if detection fails

        // block to detect the actual current terminal window size
        #if defined(_WIN32) // windows-specific window size detection
            CONSOLE_SCREEN_BUFFER_INFO csbi; // structure to hold console info
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
                term_w = csbi.srWindow.Right - csbi.srWindow.Left + 1; // calculate width
                term_h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; // calculate height
            }
        #else               // linux/macos-specific window size detection
            struct winsize w; // structure to hold window size data
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) { // get terminal size
                term_w = w.ws_col; // assign columns to width
                term_h = w.ws_row; // assign rows to height
            }
        #endif

        int size = term_w * term_h; // calculate total number of characters on screen
        std::vector<char> output(size, ' '); // char buffer initialized with spaces
        std::vector<float> zbuffer(size, 0); // z-buffer initialized with zeros for depth

        // fixed scale for the donut so it stays small regardless of screen size
        float donut_scale = 15.0f; 

        // begin 3d torus geometry calculation
        for (float theta = 0; theta < 6.28; theta += 0.07) { // loop for the cross-section circle
            float ct = cos(theta); // pre-calculate cosine of theta
            float st = sin(theta); // pre-calculate sine of theta

            for (float phi = 0; phi < 6.28; phi += 0.02) { // loop for rotating the circle
                float cp = cos(phi); // pre-calculate cosine of phi
                float sp = sin(phi); // pre-calculate sine of phi
                
                float sa = sin(a), ca = cos(a); // sine and cosine of animation angle a
                float sb = sin(b), cb = cos(b); // sine and cosine of animation angle b

                float circlex = 2 + 1 * ct; // x-coordinate of the circle before rotation
                float circley = 1 * st;     // y-coordinate of the circle before rotation

                // calculate final 3d x-coordinate after applying rotation matrices
                float x = circlex * (cb * cp + sa * sb * sp) - circley * ca * sb;
                // calculate final 3d y-coordinate after applying rotation matrices
                float y = circlex * (sb * cp - sa * cb * sp) + circley * ca * cb;
                // calculate 3d z-coordinate (depth) and push it back by 5 units
                float z = 5 + ca * circlex * sp + circley * sa;
                
                float ooz = 1 / z; // calculate inverse depth for perspective projection

                // project 3d x to 2d screen space and center it
                int xp = (int)(term_w / 2 + (donut_scale * 2) * ooz * x);
                // project 3d y to 2d screen space and center it
                int yp = (int)(term_h / 2 - donut_scale * ooz * y);

                // convert 2d screen coordinates to a 1d array index
                int idx = xp + term_w * yp;

                // calculate surface luminance (how much light hits the point)
                float L = 8 * ((st * sa - ct * cp * ca) * cb - ct * cp * sa - st * ca - ct * sp * sb);

                // check if the point is within screen bounds and visible (z-buffer test)
                if (term_h > yp && yp > 0 && xp > 0 && term_w > xp && ooz > zbuffer[idx]) {
                    zbuffer[idx] = ooz; // update the depth buffer with the new closest point
                    int l_idx = L > 0 ? (int)L : 0; // ensure luminance index is non-negative
                    if (l_idx > 11) l_idx = 11; // clamp maximum luminance to character set length
                    output[idx] = ".,-~:;=!*#$@"[l_idx]; // assign character based on brightness
                }
            }
        }

        // move the terminal cursor to the top-left corner (0,0) without clearing
        std::cout << "\x1b[H"; 
        
        // iterate through the frame buffer and print to terminal
        for (int i = 0; i < size; i++) {
            // print a newline at the end of each terminal row
            if ((i + 1) % term_w == 0) std::cout << '\n'; 
            // otherwise print the character from the buffer
            else std::cout << output[i]; 
        }

        // increment rotation angles for the next frame
        a += 0.05; // increase pitch rotation
        b += 0.03; // increase yaw rotation

        // pause execution for 30 milliseconds to maintain smooth frame rate
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return 0; // end of the program
}
