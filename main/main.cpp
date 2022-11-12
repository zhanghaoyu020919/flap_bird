#include "../lib/display.h"
#ifndef __SRC_FILE
#define __SRC_FILE
#endif  // !__SRC_FILE
#include <vector>

#include "../game/border.h"
#include "../game/movingRectangle.h"
#include "../game/rectangle.h"
#include "../lib/audio.h"
#include "bird.h"

using std::vector;
using ObjPool = vector<Object*>;

void detect_collision(ObjPool& pool) {
    // TODO: detect collision between each pair of objects
    /* Your code here */
    for(int i=0;i<(int)pool.size();i++)
    {
        for(int j=0;j<(int)pool.size();j++)
        {
            if(i!=j)
            {
                pool[i]->detect_collide(*pool[j]);
            }
                
        }
    }
    

}

void show_game_over(Display& display, double second) {
    display.clear();
    display.put_string(display.get_cols() / 2 - 7, display.get_rows() / 2 - 1,
                       "Game Over");
    display.put_string(display.get_cols() / 2 - 14, display.get_rows() / 2,
                       "Bird survives " + std::to_string(second) + " seconds");
    display.put_string(display.get_cols() / 2 - 13, display.get_rows() / 2 + 1,
                       "Press any key to exit");
    display.refresh();
    display.sleep(200);
    display.get_char();
}

void add_rectangle(ObjPool& pool, Display& d) {
    // TODO: add a moving_rectangle to the pool
    /* Your code here */
   
    
        srand((unsigned int)(time(NULL)));
        int y=rand()%(d.get_rows()-2)+1,width=rand()%5+1,height=rand()%((d.get_rows()-1-y)*2/3)+1,x=d.get_cols()-1-width;
        MovingRectangle *movingrectangle=new MovingRectangle(x,y,width,height,-1,0);
        pool.push_back(movingrectangle);
    
    
}

int main() {
    Display d("log.txt");
    string audio_dir = "../audio/";
    ObjPool obj_pool;
    Bird* bird = new Bird(d.get_cols() / 2, d.get_rows() / 2, 0, 0, 0, 0);
    Border* border = new Border(0, 0, d.get_cols(), d.get_rows());
    Audio bgm(audio_dir+"bgm.mp3");
    Audio fly(audio_dir+"flap.mp3");
    Audio collide(audio_dir+"collide.mp3");
    Audio gameover(audio_dir+"applause.mp3");
    obj_pool.emplace_back(bird);
    obj_pool.emplace_back(border);
    bgm.play_loop();
    long int count=0;
    int flag=0;
    int level=0;
    while (true) {
        // Process keyboard input
        int c = d.get_char(50);
        d.clear();
        if (c == 'q') {
            bgm.stop();
            fly.stop();
            gameover.stop();
            collide.stop();
            break;
        } else if (c == ' ') {
            if(bird->get_v().second!=-0.9)
            {
                
                fly.play_once();
            }
        }
        if(count%200==0&&level<=15)
            level+=1;
        
        

        // Add new rectangle
        if(count%(int)(120/level)==0)
            add_rectangle(obj_pool, d);

        // Update object state
        for (auto& obj : obj_pool) {
            obj->tick(c);
        }

        // Detect collision
        detect_collision(obj_pool);

        //TODO: Process invalid object
        /* Your code here */
        
        ObjPool::iterator iter=obj_pool.begin();
        
        for(int i =0;i<(int)obj_pool.size()-1;i++,iter++)
        {
            if(obj_pool[i]->get_valid()==false&&typeid(obj_pool[i]->type())!=typeid(Border))
            {     
                if(obj_pool[i]->type()==bird->type()) 
                    flag=1;
                delete []obj_pool[i];
                obj_pool.erase(iter);
            }
        }

        //TODO: Bird collide with border
        /* Your code here */
        if(flag==1)
        {
            bgm.stop();
            
            collide.play_once();
            
            gameover.play_once();
            show_game_over(d,d.time()/1000.0);
            gameover.stop();
            break;
        }

        // Draw objects
        for (auto& obj : obj_pool) {
            obj->draw(d);
        }
        d.put_string(1, 2, "Time: " + std::to_string(d.time() / 1000.0));
        d.put_string(1, 3, "Keystrock: " + std::to_string(c));
        d.put_string(1, 1, "Level: " + std::to_string(level));
        
        d.put_horizontal_line(0,0,d.get_cols(),'-');
        d.put_horizontal_line(0,d.get_rows()-1,d.get_cols(),'-');
        d.put_vertical_line(0,1,d.get_rows()-2,'|');
        d.put_vertical_line(d.get_cols()-1,1,d.get_rows()-2,'|');
        d.refresh();
        d.log();
        count++;
    } 
    return 0;
}