class Button
{
  public Button(int posX, int posY, int bWidth, int bHeight)
  {
    this.posX = posX;
    this.posY = posY;
    this.bWidth = bWidth;
    this.bHeight = bHeight;
  }


  void draw()
  {
    int tX = posX - bWidth/2,
        tY = posY;
    color currColor = (selected) ? halfColor : fullColor;
    fill(currColor);
    rect(tX, tY, bWidth, bHeight);
    fill(0);
    
    text("Generate", tX + xPadding, tY + yPadding, 2);
  }
  
  void update(int x, int y)
  {
    //println(x + ":" + (posX - bWidth/2));
    //println((posX - bWidth/2) + ":" + x + ":" + (posX + bWidth/2));
    //println((posY - bHeight) + ":" + y + ":" + (posY));
    selected = false;
    if(x <= posX - bWidth/2 || x >= posX + bWidth/2) return;
    if(y <= posY || y >= posY + bHeight) return;
    selected = true;
  }
  
  boolean checkMousePressed()
  {
    if(!selected) return false;

    println("Button Pressed");
    return true;
  }

  int posX, posY, bWidth, bHeight, xPadding = 30, yPadding = 35;
  boolean selected = false;
  color fullColor = color(255);
  color halfColor = color(200);
}