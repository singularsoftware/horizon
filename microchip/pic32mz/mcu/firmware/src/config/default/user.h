/*******************************************************************************
  User Configuration Header

  File Name:
    user.h

  Summary:
    Build-time configuration header for the user defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    It only provides macro definitions for build-time configuration options

*******************************************************************************/

#ifndef USER_H
#define USER_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: User Configuration macros
// *****************************************************************************
// *****************************************************************************
#define USER_CONSOLE_PRINT
    
#ifdef USER_CONSOLE_PRINT
    #undef USER_CONSOLE_PRINT
    #define USER_CONSOLE_PRINT(fmt, ...)    SYS_CONSOLE_PRINT(fmt, ##__VA_ARGS__)
#else
    #define USER_CONSOLE_PRINT(fmt, ...)    
#endif

#define USER_ECHO_TO_CONSOLE_PRINT
#ifdef USER_ECHO_TO_CONSOLE_PRINT
    #undef USER_ECHO_TO_CONSOLE_PRINT
    #define USER_ECHO_TO_CONSOLE_PRINT(fmt, ...)    SYS_CONSOLE_PRINT(fmt, ##__VA_ARGS__)
#else
    #define USER_ECHO_TO_CONSOLE_PRINT(fmt, ...)    
#endif

    

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
