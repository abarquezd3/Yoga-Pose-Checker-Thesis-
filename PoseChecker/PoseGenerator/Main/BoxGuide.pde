//For default positioning
public enum BoxType {TORSO, ARM, LEFTTHIGH, RIGHTTHIGH};

class BoxGuide
{
  public BoxGuide(int posX, int posY, BoxType type){
    this.posX = posX;
    this.posY = posY;
    box = loadShape(boxPath);
    //box.rotateX(radians(-60));
    //box.rotateY(radians(-30));
    defX = -90;
    switch(type)
    {
      case TORSO:        
        //box.rotateZ(radians(10));
        //box.resetMatrix();
      case LEFTTHIGH:
        defY = -90;
        break;
      case ARM:
      case RIGHTTHIGH:
        defY = 90;
        break;
    }
    
    box.rotateX(radians(defX));
    box.rotateY(radians(defY));
  }
  
  public void draw()
  {
    pushMatrix();
    translate(posX, posY, 0);
    scale(20);
    
    shape(box);
    popMatrix();
  }
  
  public void setRotation(float roll, float pitch)
  {
    this.roll = roll;
    this.pitch = (defY < 0) ? -pitch : pitch;
    
    //Reset box;
    box.resetMatrix();
    box.rotateX(radians(defX));
    box.rotateY(radians(defY));
    
    //Roll & Pitch
    box.rotateX(radians(roll));
    box.rotateZ(radians(pitch));
  }
  
  PShape box;
  int posX, posY;
  //Default rotations in degrees
  float defX, defY,
        roll = 0, pitch = 0; //Rotation by movement
  String boxPath = "Box.obj";
}