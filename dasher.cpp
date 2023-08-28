#include "raylib.h"
#include <cstdio>

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight){
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime,int maxFrame){
    //update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime){
        data.runningTime = 0.0;
        //update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame){
            data.frame = 0;
        }
    }
    return data;
}



int main(){
    //window dimensions
    const int windowWidth = 512;                                                            //game window width set to 400 px
    const int windowHeight = 380;                                                            //game window height set to 600 px
    int windowDimensions[2] = {windowWidth,windowHeight};
    //initialize the window
    InitWindow(windowDimensions[0],windowDimensions[1],"Dapper Dasher");
    
    // //acceleration due to gravity (pixels/frame)/frame
    // const int gravity{1};

    //acceleration due to gravity (pixels/sec)/sec
    const int gravity{2000};

    //rectangle dimensions
    //int r_width =  50;
    //int r_height = 80;
    //int pos_y = windowHeight - r_height;
    int velocity;          


    // //jump velocity in pixels/frame
    // const int jumpVel = -18;

    //jump velocity in pixels/sec
    const int jumpVel = -800;
    
    //is in air check
    bool isInAir{};

    //background texture
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};

    //midground texture
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};

    //foreground texture
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    int bgsVel{20};


    //Scarfy texture
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // Rectangle scarfyRec;
    // scarfyRec.width = scarfy.width/6;
    // scarfyRec.height = scarfy.height;
    // scarfyRec.x = 0;
    // scarfyRec.y = 0;
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;

    //Vector2 scarfyPos;
    // scarfyPos.x = windowWidth/2 - scarfyRec.width/2;
    // scarfyPos.y = windowHeight - scarfyRec.height;
    scarfyData.pos.x = windowWidth/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowHeight - scarfyData.rec.height;
    //int frame = 0;
    scarfyData.frame = 0;
    


    //Nebula texture
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    const int no_of_nebulae = 8;
    // //AnimData for nebula
    // AnimData nebData{ 
    //     {0.0,0.0,nebula.width/8,nebula.height/8} ,     //Rectangle rec
    //     {windowWidth,windowHeight-nebula.height/8},    //Vector2  pos
    //     0,                                             //Int frame
    //     1.0/12.0,                                      //Float updateTime
    //     0                                              //Float runningTime
    // };
    // AnimData neb2Data{ 
    //     {0.0,0.0,nebula.width/8,nebula.height/8} ,           //Rectangle rec
    //     {windowWidth + 300,windowHeight-nebula.height/8},    //Vector2  pos
    //     0,                                                   //Int frame
    //     1.0/16.0,                                            //Float updateTime
    //     0                                                    //Float runningTime
    // };

    AnimData nebulae[no_of_nebulae]{};

    for(int i = 0; i < no_of_nebulae; i++){
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = windowHeight-nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].runningTime = 0.0;
        nebulae[i].updateTime = 0.0;
        nebulae[i].pos.x = windowDimensions[0] + (i*300);
    }

    float finishLine{nebulae[no_of_nebulae - 1].pos.x};
   
    //nebula x velocity (pixels/second)
    int nebVel{-250};
    

    //amount of time before we update the scarfy animation frame
    //const float updateTime = 1.0/12.0;
    scarfyData.updateTime = 1.0/12.0;
    //float runningTime{};
    scarfyData.runningTime = 0.0;
    //amount of time before we update the nebula animation frame
    // const float nebUpdateTime = 1.0/24.0;
    // float nebRunningTime{};

    // int neb2Frame{};
    // const float neb2UpdateTime{1.0/16.0};
    // float neb2RunningTime;

    //setting target fps of the game to 60 fps 
    SetTargetFPS(60);  
    bool collision{};                                                      
    while(!WindowShouldClose()){
        //start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //getting the delta time(time between frames)
        const float dT = GetFrameTime();

        //move background
        bgX -= bgsVel * dT;
        if(bgX <= -background.width*2){
            bgX = 0;
        }
        //move midground
        mgX -= (bgsVel*2) * dT;
        if(mgX <= -midground.width*2){
            mgX = 0;
        }
        //move foreground
        fgX -= (bgsVel * 4) * dT;
        if(fgX <= -foreground.width*2){
            fgX = 0;
        }
        //draw the background
        Vector2 bg1Pos{bgX,0.0};
        DrawTextureEx(background,bg1Pos,0.0,2.0,WHITE);
        Vector2 bg2Pos{bgX + background.width*2,0.0};
        DrawTextureEx(background,bg2Pos,0.0,2.0,WHITE);

        // //draw the midground
        Vector2 mg1Pos{mgX,0.0};
        DrawTextureEx(midground,mg1Pos,0.0,2.0,WHITE);
        Vector2 mg2Pos{mgX + midground.width*2,0.0};
        DrawTextureEx(midground,mg2Pos,0.0,2.0,WHITE);

        // //draw the foreground
        Vector2 fg1Pos{fgX,0.0};
        DrawTextureEx(foreground,fg1Pos,0.0,2.0,WHITE);
        Vector2 fg2Pos{fgX + foreground.width*2,0.0};
        DrawTextureEx(foreground,fg2Pos,0.0,2.0,WHITE);
       
        //game logic starts down here 

        if(isOnGround(scarfyData,windowDimensions[1])){
            velocity = 0;
            isInAir = false;
        }else{
            velocity += gravity * dT;
            isInAir = true;
        }

        if(IsKeyPressed(KEY_SPACE) && !isInAir){
            velocity += jumpVel ;               
        }

        //update scarfy position
        scarfyData.pos.y +=velocity * dT;
        
        //update nebulae positions
        for(int i = 0; i < no_of_nebulae; i++){
            nebulae[i].pos.x += nebVel * dT;
            
        }
        //update finish line    
        finishLine += nebVel * dT;
  
        //update scarfy animation frame
        if(!isInAir){
            scarfyData = updateAnimData(scarfyData,dT,5);
        }

        //update nebulae animation frame
        for(int i = 0; i < no_of_nebulae; i++){
            nebulae[i] = updateAnimData(nebulae[i],dT,7);
        }
        

        for(AnimData nebula : nebulae){
            float pad{60};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad 
            };
            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };
            if(CheckCollisionRecs(nebRec,scarfyRec)){
                collision = true;
            }
        }
        
        if(collision){
            //lose the game
            DrawText("GAME OVER",(windowWidth/2)-150,windowHeight/2-100,50,WHITE);
            DrawText("Press 'R' to restart",(windowWidth/2)-120,windowHeight/2 + 100,25,WHITE);
            if(IsKeyPressed(KEY_R)){
                for(int i = 0; i < no_of_nebulae; i++){
                    nebulae[i].pos.x = windowDimensions[0] + (i*300);
                }
                finishLine = nebulae[no_of_nebulae - 1].pos.x;
                collision = false;
            }
            
        }else if(scarfyData.pos.x >= finishLine + 200){
            DrawText("3a4 yaba",((windowWidth/2)/2)-30,windowHeight/2 - 40,70,WHITE);
        }else{
            //draw scarfy
            DrawTextureRec(scarfy,scarfyData.rec,scarfyData.pos,WHITE);
            //draw the nebulas
            for(int i = 0; i < no_of_nebulae; i++){
                DrawTextureRec(nebula,nebulae[i].rec,nebulae[i].pos,WHITE);
            }

        }
        
        //Stop drawing
        EndDrawing();

    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}








// //update running time
// scarfyData.runningTime += dT;
// if (scarfyData.runningTime >=scarfyData.updateTime && !isInAir){
//     scarfyData.runningTime = 0.0;
//     //updating the animation frame
//     scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
//     scarfyData.frame++;
//     if(scarfyData.frame > 5){
//         scarfyData.frame = 0;
//     }
// }

// for(int i = 0; i < no_of_nebulae; i++){
// //update nebula running time
// nebulae[i].runningTime += dT;
// if (nebulae[i].runningTime >=nebulae[i].updateTime ){
//     nebulae[i].runningTime = 0.0;
//     //updating the animation frame
//     nebulae[i].rec.x = nebulae[i].frame * nebulae[i].rec.width;
//     nebulae[i].frame++;
//     if(nebulae[i].frame > 7){
//         nebulae[i].frame = 0;
//     }
// }
// }