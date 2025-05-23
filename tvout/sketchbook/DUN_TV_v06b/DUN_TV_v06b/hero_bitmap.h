#include <avr/pgmspace.h>

//Start of Hero bitmaps
PROGMEM prog_uchar character_bitmap[] = {
  8,8,

  // begin Elf
  0b00000000, //         
  0b01010010, //  # #  # 
  0b11111110, // ####### 
  0b01111110, //  ###### 
  0b11110100, // #### #  
  0b00010000, //    #    
  0b01010100, //  # # #  
  0b00101000, //   # #   
  
  0b00000000, //         
  0b00111001, //   ###  #
  0b01011111, //  # #####
  0b11111111, // ########
  0b01111010, //  #### # 
  0b01010000, //  # #    
  0b00100000, //   #     
  0b00011000, //    ##   
  
  0b00000000, //         
  0b01110010, //  ###  # 
  0b11111110, // ####### 
  0b01011110, //  # #### 
  0b11110100, // #### #  
  0b01000000, //  #      
  0b00100000, //   #     
  0b00000000, //         
  
  0b00000000, //         
  0b00110010, //   ##  # 
  0b11111110, // ####### 
  0b01110110, //  ### ## 
  0b11111100, // ######  
  0b00010000, //    #    
  0b01101100, //  ## ##  
  0b00000000, //         
  
  0b00000000, //         
  0b00111001, //   ###  #
  0b11111111, // ########
  0b01111011, //  #### ##
  0b11111110, // ####### 
  0b00011000, //    ##   
  0b00111000, //   ###   
  0b00000000, //         
  
  0b00000000, //         
  0b00110010, //   ##  # 
  0b11011110, // ## #### 
  0b01110110, //  ### ## 
  0b11111100, // ######  
  0b01110000, //  ###    
  0b01100000, //  ##     
  0b00000000, //         
  
  0b00101000, //   # #   
  0b01010100, //  # # #  
  0b00010000, //    #    
  0b11110100, // #### #  
  0b01111110, //  ###### 
  0b11111110, // ####### 
  0b01110010, //  ###  # 
  0b00000000, //         
  
  0b00011000, //    ##   
  0b00100000, //   #     
  0b01010000, //  # #    
  0b01111010, //  #### # 
  0b11111111, // ########
  0b01111111, //  #######
  0b00111001, //   ###  #
  0b00000000, //         
  
  0b00000000, //         
  0b00100000, //   #     
  0b01000000, //  #      
  0b11110100, // #### #  
  0b00111110, //   ##### 
  0b11111110, // ####### 
  0b01110010, //  ###  # 
  0b00000000, //         
  
  0b00000000, //         
  0b01101100, //  ## ##  
  0b00010000, //    #    
  0b10111100, // # ####  
  0b01011110, //  # #### 
  0b11101110, // ### ### 
  0b00110010, //   ##  # 
  0b00000000, //         
  
  0b00000000, //         
  0b00111000, //   ###   
  0b00011000, //    ##   
  0b11011110, // ## #### 
  0b01101111, //  ## ####
  0b11110111, // #### ###
  0b00111001, //   ###  #
  0b00000000, //         
  
  0b00000000, //         
  0b01100000, //  ##     
  0b01110000, //  ###    
  0b10111100, // # ####  
  0b01011110, //  # #### 
  0b11101110, // ### ### 
  0b00110010, //   ##  # 
  0b00000000, //         
  // end Elf

  // begin Warrior
  0b01110000, //  ###    
  0b01010000, //  # #    
  0b11111110, // ####### 
  0b01111100, //  #####  
  0b01110110, //  ### ## 
  0b11100100, // ###  #  
  0b00100000, //   #     
  0b01011100, //  # ###  
  
  0b00000000, //         
  0b01110000, //  ###    
  0b01011111, //  # #####
  0b11111110, // ####### 
  0b11111011, // ##### ##
  0b01110010, //  ###  # 
  0b00010000, //    #    
  0b00001000, //     #   
  
  0b00000000, //         
  0b00000000, //         
  0b11111110, // ####### 
  0b01011100, //  # ###  
  0b01110110, //  ### ## 
  0b11100100, // ###  #  
  0b01000000, //  #      
  0b01000000, //  #      
  
  0b01110000, //  ###    
  0b01111110, //  ###### 
  0b11110100, // #### #  
  0b01100100, //  ##  #  
  0b11110100, // #### #  
  0b00111110, //   ##### 
  0b01011100, //  # ###  
  0b00000000, //         
  
  0b01010000, //  # #    
  0b01111111, //  #######
  0b11111010, // ##### # 
  0b01110010, //  ###  # 
  0b11111010, // ##### # 
  0b00111111, //   ######
  0b01011000, //  # ##   
  0b00000000, //         
  
  0b01110000, //  ###    
  0b01011110, //  # #### 
  0b11110100, // #### #  
  0b01100100, //  ##  #  
  0b11110100, // #### #  
  0b00111110, //   ##### 
  0b01000000, //  #      
  0b00000000, //         
  
  0b01011100, //  # ###  
  0b00100000, //   #     
  0b11100100, // ###  #  
  0b01110110, //  ### ## 
  0b01111100, //  #####  
  0b11111110, // ####### 
  0b01110000, //  ###    
  0b01110000, //  ###    
  
  0b00001000, //     #   
  0b00010000, //    #    
  0b01010010, //  # #  # 
  0b11111011, // ##### ##
  0b11111110, // ####### 
  0b01111111, //  #######
  0b01110000, //  ###    
  0b00000000, //         
  
  0b01000000, //  #      
  0b01000000, //  #      
  0b10100100, // # #  #  
  0b00110110, //   ## ## 
  0b01111100, //  #####  
  0b11111110, // ####### 
  0b00000000, //         
  0b00000000, //         
  
  0b00000000, //         
  0b01011100, //  # ###  
  0b00111110, //   ##### 
  0b11111100, // ######  
  0b01111100, //  #####  
  0b11111100, // ######  
  0b01111110, //  ###### 
  0b01110000, //  ###    
  
  0b00000000, //         
  0b01011000, //  # ##   
  0b00111111, //   ######
  0b11111110, // ####### 
  0b01111110, //  ###### 
  0b11111110, // ####### 
  0b00111111, //   ######
  0b01110000, //  ###    
  
  0b00000000, //         
  0b01000000, //  #      
  0b00111110, //   ##### 
  0b11111100, // ######  
  0b01111100, //  #####  
  0b11111100, // ######  
  0b01111110, //  ###### 
  0b01110000, //  ###    
  // end Warrior

  // begin Wizard
  0b00111000, //   ###   
  0b11111100, // ######  
  0b10111110, // # ##### 
  0b11111110, // ####### 
  0b11110110, // #### ## 
  0b01100100, //  ##  #  
  0b00010000, //    #    
  0b11101110, // ### ### 
  
  0b00011100, //    ###  
  0b11111110, // ####### 
  0b11111111, // ########
  0b10111111, // # ######
  0b11111011, // ##### ##
  0b01110010, //  ###  # 
  0b11101110, // ### ### 
  0b00000000, //         
  
  0b00111000, //   ###   
  0b11111100, // ######  
  0b11111110, // ####### 
  0b11111110, // ####### 
  0b11010110, // ## # ## 
  0b01100100, //  ##  #  
  0b00010000, //    #    
  0b00001000, //     #   
  
  0b00000000, //         
  0b00111100, //   ####  
  0b11110110, // #### ## 
  0b11100110, // ###  ## 
  0b11110110, // #### ## 
  0b01111100, //  #####  
  0b00010000, //    #    
  0b11101110, // ### ### 
  
  0b00000000, //         
  0b00111110, //   ##### 
  0b11111011, // ##### ##
  0b11110011, // ####  ##
  0b11111011, // ##### ##
  0b01111110, //  ###### 
  0b00010000, //    #    
  0b11101110, // ### ### 
  
  0b00000000, //         
  0b00111100, //   ####  
  0b11010110, // ## # ## 
  0b11100110, // ###  ## 
  0b11110110, // #### ## 
  0b01111100, //  #####  
  0b00100000, //   #     
  0b11011000, // ## ##   
  
  0b11101110, // ### ### 
  0b00010000, //    #    
  0b01100100, //  ##  #  
  0b11110110, // #### ## 
  0b11111110, // ####### 
  0b11111110, // ####### 
  0b11111100, // ######  
  0b00111000, //   ###   
  
  0b00000000, //         
  0b11101110, // ### ### 
  0b01110010, //  ###  # 
  0b11111011, // ##### ##
  0b11111111, // ########
  0b11111111, // ########
  0b11111110, // ####### 
  0b00011100, //    ###  
  
  0b00001000, //     #   
  0b00010000, //    #    
  0b01000100, //  #   #  
  0b10110110, // # ## ## 
  0b01111110, //  ###### 
  0b11111110, // ####### 
  0b11111100, // ######  
  0b00111000, //   ###   
  
  0b11101110, // ### ### 
  0b00010000, //    #    
  0b01111100, //  #####  
  0b11010110, // ## # ## 
  0b11101110, // ### ### 
  0b11010110, // ## # ## 
  0b00111100, //   ####  
  0b00000000, //         
  
  0b11101110, // ### ### 
  0b00010000, //    #    
  0b01111110, //  ###### 
  0b11101011, // ### # ##
  0b11110111, // #### ###
  0b11101011, // ### # ##
  0b00111110, //   ##### 
  0b00000000, //         
  
  0b11011000, // ## ##   
  0b00100000, //   #     
  0b01111100, //  #####  
  0b11010110, // ## # ## 
  0b11101110, // ### ### 
  0b11010110, // ## # ## 
  0b00111100, //   ####  
  0b00000000, //         

  // end Wizard

  // begin Valyrie
  0b00011000, //    ##   
  0b10001100, // #   ##  
  0b01011110, //  # #### 
  0b01111110, //  ###### 
  0b10110010, // # ##  # 
  0b00100000, //   #     
  0b01011000, //  # ##   
  0b00000000, //         
  
  0b00001100, //     ##  
  0b00000110, //      ## 
  0b00111111, //   ######
  0b11011111, // ## #####
  0b00111001, //   ###  #
  0b00100000, //   #     
  0b00010000, //    #    
  0b00000000, //         
  
  0b00011000, //    ##   
  0b10001100, // #   ##  
  0b01111110, //  ###### 
  0b01111110, //  ###### 
  0b10010010, // #  #  # 
  0b00100000, //   #     
  0b00100000, //   #     
  0b00000000, //         
  
  0b00010000, //    #    
  0b00101100, //   # ##  
  0b11110110, // #### ## 
  0b01110010, //  ###  # 
  0b11110010, // ####  # 
  0b00100000, //   #     
  0b01011000, //  # ##   
  0b00000000, //         
  
  0b00001000, //     #   
  0b00100110, //   #  ## 
  0b11111011, // ##### ##
  0b00111001, //   ###  #
  0b11111001, // #####  #
  0b00110000, //   ##    
  0b00100000, //   #     
  0b00000000, //         
  
  0b00010000, //    #    
  0b00001100, //     ##  
  0b11010110, // ## # ## 
  0b01110010, //  ###  # 
  0b11110010, // ####  # 
  0b00100000, //   #     
  0b00000000, //         
  0b00000000, //         
  
  0b00000000, //         
  0b01011000, //  # ##   
  0b00100000, //   #     
  0b10110010, // # ##  # 
  0b01111110, //  ###### 
  0b01111110, //  ###### 
  0b10001100, // #   ##  
  0b00011000, //    ##   
  
  0b00000000, //         
  0b00010000, //    #    
  0b00100000, //   #     
  0b01111001, //  ####  #
  0b11111111, // ########
  0b00111111, //   ######
  0b00000110, //      ## 
  0b00001100, //     ##  
  
  0b00000000, //         
  0b00100000, //   #     
  0b00100000, //   #     
  0b10010010, // #  #  # 
  0b01011110, //  # #### 
  0b01111110, //  ###### 
  0b10001100, // #   ##  
  0b00011000, //    ##   
  
  0b00000000, //         
  0b01011000, //  # ##   
  0b00100000, //   #     
  0b11111110, // ####### 
  0b01111110, //  ###### 
  0b11111110, // ####### 
  0b00101100, //   # ##  
  0b00010000, //    #    
  
  0b00000000, //         
  0b00100000, //   #     
  0b00110000, //   ##    
  0b11111111, // ########
  0b00111111, //   ######
  0b11111111, // ########
  0b00100110, //   #  ## 
  0b00001000, //     #   
  
  0b00000000, //         
  0b00000000, //         
  0b00100000, //   #     
  0b11111110, // ####### 
  0b01111110, //  ###### 
  0b11111110, // ####### 
  0b00001100, //     ##  
  0b00010000, //    #    
  // end Valkyrie
};


