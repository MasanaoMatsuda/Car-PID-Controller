#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "pid_counterclockwise.hpp"
#include "twiddle.hpp"


// for convenience
using nlohmann::json;
using std::string;

// 1. Tuneing only KP best
//static const double START_KP = 0.044333;

// 2. Tuneing KP and KD
//static const double START_KP = 0.04433;
//static const double START_KD = 0.01;

// 3. Tuneing KI
//static const double START_KP = 0.04433;
//static const double START_KD = 0.009;
//static const double START_KI = 0.000000001;

//static const double START_KP = 0.046564;
static const double START_KP = 0.044383;
static const double START_KD = 0.366334;
static const double START_KI = 0.000036;

static const double SPEED_MAX = 100.0;
static const double MAX_STEERING_ANGLE = 1;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}



int main() {
  uWS::Hub h;

  PIDCounterclockwize pid(START_KP, START_KI, START_KD);
  PIDCounterclockwize pid_vel(0.397, 0.0, 0.015);
  Twiddle twiddle(START_KP, START_KI, START_KD);
  double cte;
  double speed;
  double angle;
  double steer_angle;
  double throttle = 0.3;

  h.onMessage([&pid, &pid_vel, &twiddle, &cte, &speed, &angle, &steer_angle, &throttle]
  (uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          cte = std::stod(j[1]["cte"].get<string>());
          speed = std::stod(j[1]["speed"].get<string>());
          angle = std::stod(j[1]["steering_angle"].get<string>());
          
//          if (!twiddle.is_optimized)
//            if (twiddle.optimize(&cte))
//            {
//              pid.Init(twiddle.params[0], twiddle.params[1], twiddle.params[2]);
//              is_slow_computing = true;
//            }
          //std::cout << cte << std::endl;
          pid.UpdateError(&cte);
          steer_angle = pid.TotalError();
    
          // Speed Adjust Try
          pid_vel.UpdateError(&cte);
          double pid_error = pid_vel.TotalError();
          double threshold = 0.8;
          //std::cout << "PID: " << pid_error << "\tCTE: " << cte << std::endl;
          if (speed > 9 && (pid_error > threshold || pid_error < -threshold))
          {
            throttle = -pow(speed / SPEED_MAX, 2);
            if (pid_error < -2.0 || pid_error > 2.0)
              steer_angle += pid_error * 0.7;
            else if (pid_error < -1.0 || pid_error > 1.0)
              steer_angle += pid_error * 0.3;
            else
              steer_angle += pid_error >= 0 ? -0.08 : 0.08;
          }
          else
            throttle = 0.3;

          // DEBUG
          //std::cout << "CTE: " << cte << " Steering Value: " << steer_value << " Steering Angle: " << angle << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_angle;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          //std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}