import processing.serial.*;

void setup()
{
  size(900, 900, P3D);
  frameRate(50);
  ortho();
  boxes = new BoxGuide[4];
  generate = new Button(width/2, height-100, 190, 50);
  values = new float[4][2];
  
  tl = new BoxGuide(width/4, height/4, BoxType.TORSO);
  boxes[0] = tl;
  tr = new BoxGuide(width/2 + width/4, height/4, BoxType.ARM);
  boxes[1] = tr;
  bl = new BoxGuide(width/4, height/2 + height/4, BoxType.LEFTTHIGH);
  boxes[2] = bl;
  br = new BoxGuide(width/2 + width/4, height/2 + height/4, BoxType.RIGHTTHIGH);
  boxes[3] = br;
  
  //Establish Serial Connection
  port = new Serial(this, "COM3", 9600);
  port.bufferUntil('\n');
  
  setupData();
  
  //TESTING AREA
  //boxes[0].setRotation(81.40, 8.60);
}

void draw()
{
  background(0);
  drawHUD(); 
  generate.update(mouseX, mouseY);
  generate.draw();
  
  for(BoxGuide box : boxes)
  {
    box.draw();
  }
}

//Reads Lines from Serial Port
void serialEvent(Serial myPort)
{
  //serial = "non";
  buffer = myPort.readStringUntil('\n');
  if(buffer != null)
  {
    buffer = trim(buffer);
    String items[] = split(buffer, ',');
    if(items.length <= 1) return;
    
    for(int i = 0; i < 4; i++)
    {
      for(int j = 0; j < 2; j++)
      {
        values[i][j] = float(items[i * 2 + j]);
      }
      //1st value is roll, 2nd value is pitch
      boxes[i].setRotation(values[i][0], values[i][1]);
    }
    //9th value is confirmation
     lit = boolean(items[8]);
  }
 
  
}


void drawHUD()
{
  int padding = 30;
  //Draw Lines
  fill(255);
  stroke(255);
  line(0, height/2, -1, width, height/2, -1);
  line(width/2, 0, -1, width/2, height, -1);
  //Draw Text
  textSize(30);
  text("Torso", padding/2, padding); //Top Left
  text("Arm", width/2 + padding/2, padding); //Top Right
  text("Left Thigh", padding/2, height/2 + padding); //Bottom Left
  text("Right Thigh", width/2 + padding/2, height/2 + padding); //Bottom Left
  
  //Checker Circle (Turns green)
  int rad = 20;
  if(lit) fill(50, 255, 50);
  else fill(255, 50, 50);
  noStroke();
  ellipse(width/2, height/2 - 80, rad*2, rad*2);
  
  //Value
  //Top Left torso
  fill(255);
  text(values[0][0], 0, height/2 - 10);
  text(values[0][1], width/2 - 150, height/2 - 10);
  //Top Right Arm
  text(values[1][0], width/2, height/2 - 10);
  text(values[1][1], width - 150, height/2 - 10); 
  //Bottom Left Left Thigh
  text(values[2][0], 0, height - 10);
  text(values[2][1], width/2 - 150, height - 10);
  //Bottom Right Right Thigh
  text(values[3][0], width/2, height - 10);
  text(values[3][1], width - 150, height - 10);
}

void mousePressed()
{
  if(generate.checkMousePressed()) writeData();
}

//Init file writer in the sketch directory
void setupData()
{
  output = createWriter("Poses.txt");
  
}

//Append Text to writer
void writeData()
{
  String data = "{";
  //Generate files
  for(int i = 0; i < 4; i++)
  {
    for(int j = 0; j < 2; j++)
    {
      data += values[i][j];
      if(i != 3 || j != 1) data += ",";
    }
  }
  data += "}";
  
  output.println(data);
  output.flush();
}


//Boxes
BoxGuide tl, tr, bl, br; //Top Left, Top Right, Bottom Left, Bottom Right
BoxGuide[] boxes;
Button generate;

//Data
boolean lit = false; //sets the circle to green
String buffer;
float[][] values;
PrintWriter output;
Serial port;