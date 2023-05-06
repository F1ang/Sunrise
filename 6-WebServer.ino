#include <WiFi.h>
#include <WebServer.h> //引入相应库

const char *ssid = "F1ang";
const char *password = "201913018";

WebServer server(80); //声明WebServer对象

const char *username = "fang";     //用户名
const char *userpassword = "1234"; //用户密码

//HTML
String ControlPage =
    String("") +
    "<html>" +
    "<head>" +
    "    <meta charset=\"utf-8\">" +
    "    <title>ESP32 Controler</title>" +
    "    <script>" +
    "        function SetData() {" +
    "            var xmlhttp;" +
    "            if (window.XMLHttpRequest) {" +
    "                xmlhttp = new XMLHttpRequest();" +
    "            }" +
    "            else {" +
    "                xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");" +
    "            }" +
    "            xmlhttp.onreadystatechange = function () {" +
    "                if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {" +
    "                    document.getElementById(\"savesetnumber\").innerHTML = xmlhttp.responseText;" +
    "                }" +
    "            }," +
    "                xmlhttp.open(\"GET\", \"SetNumber\", true); " +
    "            xmlhttp.send();" +
    "        }" +
    "    </script>" +
    "</head>" +
    "<body>" +
    "    <div id=\"savesetnumber\">0</div>" +
    "    <input type=\"text\" name=\"in1\">"+
    "    <input type=\"submit\" name=\"sub\" onclick=\"SetData()\">"+
    "</body>" +
    "</html>";

int16_t tag_num = -2;

void handleRoot() //回调函数
{
  if (!server.authenticate(username, userpassword)) //校验用户是否登录
  {
    return server.requestAuthentication(); //请求进行用户登录认证
  }
  server.send(200, "text/html", ControlPage); //！！！注意返回网页需要用"text/html" ！！！ //登录成功切换显示
}

void handleSetNumber() //回调函数
{
  tag_num += 1;
  if (tag_num >4)tag_num = 0;
  String message = "tag_num：";
  message += String(tag_num); 
  server.send(200, "text/plain", message); //将消息发送回页面
  //Serial.println(tag_num);
}

/* SR04 */
const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
int16_t distance_flag;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200);
  /* SR04 */
  pinMode(trigPin, OUTPUT);  // 将trigPin设置为输出
  pinMode(echoPin, INPUT);   // 将echoPin设置为输入

  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected");
  Serial.print("IP Address:");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot); //注册链接和回调函数
  server.on("/SetNumber", HTTP_GET, handleSetNumber); //注册网页中ajax发送的get方法的请求和回调函数
  
  server.begin(); //启动服务器
  Serial.println("Web server started");
}

//读取11byte 0x55 0x53 Roll_L Roll_H Pitch_L Pitch_H Yaw_L Yaw_H V_L V_H Sum
byte sendData[11] = {0x55,0x53,0,0,0,0,0,0,0,0,0};


void loop()
{
  server.handleClient(); //处理来自客户端的请求

  /* SR04 */
  digitalWrite(trigPin, LOW);  
  delayMicroseconds(2);  
  //将trigPin设置为HIGH状态10微秒
  digitalWrite(trigPin, HIGH);  
  delayMicroseconds(10);  
  digitalWrite(trigPin, LOW);   
  //读取echoPin，返回声波传播时间(微秒)  
  duration = pulseIn(echoPin, HIGH);    
  //计算距离  
  distanceCm = duration * SOUND_SPEED/2;     
  
  if (distanceCm < 15) distance_flag = 1;
  else distance_flag = 0;
  
  Serial.println(distanceCm);
  Serial.println(tag_num);
  Serial.println(distance_flag);
  sendData[2] = tag_num;
  sendData[3] = 0;
  sendData[4] = distance_flag;
  sendData[5] = 0;
  Serial2.write(sendData, 11);
  
}
