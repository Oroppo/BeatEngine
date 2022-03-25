// We need the flags from the frame uniforms
#include "frame_uniforms.glsl"

// Our color correction 3d texture
uniform layout (binding=14) sampler3D s_ColorCorrection;
uniform layout (binding=16) sampler3D s_ColorCorrectionCool;
uniform layout (binding=17) sampler3D s_ColorCorrectionCustom;

// Function for applying color correction
vec3 ColorCorrect(vec3 inputColor) {
   // If our color correction flag is set, we perform the color lookup
    if (IsFlagSet(FLAG_ENABLE_COLOR_CORRECTION)) {
        if(u_toggleKeys == 8.0)
        {
        return texture(s_ColorCorrection, inputColor).rgb;
        }
        
        else if(u_toggleKeys == 9.0)
        {
        return texture(s_ColorCorrectionCool, inputColor).rgb;
        }
        else if(u_toggleKeys == 0.0)
        {
        return texture(s_ColorCorrectionCustom, inputColor).rgb;
        }
        
        else 
        {
        return inputColor;
        }
    }
    // Otherwise just return the input
   // else {
    //    return inputColor;
   // }
}

