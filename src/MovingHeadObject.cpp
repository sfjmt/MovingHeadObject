#include "MovingHeadObject.h"

MovingHeadObject::MovingHeadObject()
{
    _angle_pan  = 0;
    _angle_tilt = 0;
    
    _posX_sphere = 0;
    _posY_sphere = 0;
    _posZ_sphere = 0;
    
    _translateX = 0;
    _translateY = 0;
    _translateZ = 0;
    
    _rotateX = 0;
    _rotateY = 0;
    _rotateZ = 0;
    
    _lineWidth = 2;
    
    _getRawPosition = ofVec3f(0, 0, 0);
    _targetPosition = ofVec3f(0, 0, 0);
    _AXIS_X_NORMAL  = ofVec3f(1, 0, 0);
    _AXIS_Y_NORMAL  = ofVec3f(0, 1, 0);
    _AXIS_Z_NORMAL  = ofVec3f(0, 0, 1);
    
    _objectColor  = ofColor(0,100);
    _monitorColor = ofColor(255,180,0,200);
    _lineColor    = ofColor(255,100);
    
    _objectScale = 1.0;
    
    DEBUG = false;
    
    PAN_MIN  = 0;
    PAN_MAX  = 540;
    TILT_MIN = -35;
    TILT_MAX = 215;
}

MovingHeadObject::~MovingHeadObject()
{
    
}

//--------------------------------------------------------------
ofVec3f MovingHeadObject::getPosition()
{
    return ofVec3f(_translateX,_translateY,_translateZ);
}

//--------------------------------------------------------------
int MovingHeadObject::getAnglePan()
{
    return _angle_pan;
}

//--------------------------------------------------------------
int MovingHeadObject::getAngleTilt()
{
    return _angle_tilt;
}

//--------------------------------------------------------------
void MovingHeadObject::setRotateX(int val)
{
    _rotateX = val;
}

//--------------------------------------------------------------
void MovingHeadObject::setRotateY(int val)
{
    _rotateY = val;
}

//--------------------------------------------------------------
void MovingHeadObject::setRotateZ(int val)
{
    _rotateZ = val;
}

//--------------------------------------------------------------
void MovingHeadObject::setPanMin(int angle)
{
    PAN_MIN = angle;
}

//--------------------------------------------------------------
void MovingHeadObject::setPanMax(int angle)
{
    PAN_MAX = angle;
}

//--------------------------------------------------------------
void MovingHeadObject::setTiltMin(int angle)
{
    TILT_MIN = angle;
}

//--------------------------------------------------------------
void MovingHeadObject::setTiltMax(int angle)
{
    TILT_MAX = angle;
}

//--------------------------------------------------------------
void MovingHeadObject::setName(string name)
{
    MY_NAME = name;
}

//--------------------------------------------------------------
void MovingHeadObject::setObjectColor(int r, int g, int b, int a)
{
    _objectColor = ofColor(r,g,b,a);
}

//--------------------------------------------------------------
void MovingHeadObject::setObjectColor(int gray, int a)
{
    _objectColor = ofColor(gray,gray,gray,a);
}

//--------------------------------------------------------------
void MovingHeadObject::setMonitorColor(int r, int g, int b, int a)
{
    _monitorColor = ofColor(r,g,b,a);
}

//--------------------------------------------------------------
void MovingHeadObject::setLineColor(int r, int g, int b, int a)
{
    _lineColor = ofColor(r,g,b,a);
}

//--------------------------------------------------------------
void MovingHeadObject::setLineColor(int gray, int a)
{
    _lineColor = ofColor(gray,gray,gray,a);
}

//--------------------------------------------------------------
void MovingHeadObject::setLineWidth(int lineWidth)
{
    _lineWidth = lineWidth;
}

//--------------------------------------------------------------
void MovingHeadObject::setRotatePan(int angle)
{
    _angle_pan = angle;
}

//--------------------------------------------------------------
void MovingHeadObject::setRotateTilt(int angle)
{
    _angle_tilt = angle;
}

//--------------------------------------------------------------
void MovingHeadObject::setPosition(ofVec3f v)
{
    _translateX = v.x;
    _translateY = v.y;
    _translateZ = v.z;
}

//--------------------------------------------------------------
void MovingHeadObject::setChasePosition(ofVec3f v)
{
    _getRawPosition = v;
    
    ofQuaternion qr;
    ofQuaternion qp;
    ofQuaternion qy;
    
    qr = ofQuaternion(-_rotateX, _AXIS_X_NORMAL);
    qp = ofQuaternion(-_rotateY, _AXIS_Y_NORMAL);
    qy = ofQuaternion(-_rotateZ, _AXIS_Z_NORMAL);
    
    ofQuaternion qt;// total quaternion
    
    qt = qr * qp * qy;
    
    ofMatrix4x4 m;
    m.setRotate(qt);
    
    ofVec3f translate_v = ofVec3f(v.x - _translateX,
                                  v.y - _translateY,
                                  v.z - _translateZ);
    
    _targetPosition = translate_v * m;
    
    _targetPosition = ofVec3f(_targetPosition.x,
                              _targetPosition.y,
                              _targetPosition.z);
    
    ofVec3f v1 = ofVec3f(0, 0, 0);
    ofVec3f v2 = ofVec3f(_targetPosition.x, _targetPosition.y, _targetPosition.z);
    ofVec3f v3 = ofVec3f(_targetPosition.x, 0, _targetPosition.z);
    
    _angle_pan  = panAngle(v3);
    _angle_tilt = tiltAngle(v1, v2, v3);
}

//--------------------------------------------------------------
void MovingHeadObject::setup(float objectScale)
{
    _objectScale = objectScale;
    
    _obj_pan = new ofBoxPrimitive();
    _obj_pan->set(200*_objectScale, 200*_objectScale, 100*_objectScale);
    _obj_pan->rotate(90, 1.0, 0.0, 0.0);
    
    _obj_tilt = new ofCylinderPrimitive();
    _obj_tilt->set(100*_objectScale, 300*_objectScale);
    _obj_tilt->rotate(90, 0.0, 0.0, 1.0);
    
    _obj_base = new ofBoxPrimitive();
    _obj_base->set(300*_objectScale, 300*_objectScale, 100*_objectScale);
    _obj_base->rotate(90, 1.0, 0.0, 0.0);
    
    _obj_monitor = new ofBoxPrimitive();
    _obj_monitor->set(200*_objectScale, 50*_objectScale, 20*_objectScale);
}

//--------------------------------------------------------------
void MovingHeadObject::debug(bool flag)
{
    DEBUG = flag;
}

//--------------------------------------------------------------
void MovingHeadObject::update()
{
    
}

//--------------------------------------------------------------
void MovingHeadObject::draw()
{
    ofPushMatrix();
    
    ofSetColor(_lineColor);
    ofSetLineWidth(_lineWidth);
    ofLine(_translateX, _translateY, _translateZ, _getRawPosition.x, _getRawPosition.y, _getRawPosition.z);
    
    ofTranslate(_translateX, _translateY, _translateZ);
    
    ofRotateX(_rotateX);
    ofRotateY(_rotateY);
    ofRotateZ(_rotateZ);
    
    draw_pan_status();
    draw_tilt_status();
    draw_base_status();
    draw_monitor_status();
    
    if(DEBUG)ofDrawAxis(300);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void MovingHeadObject::draw(ofVec3f v)
{
    _translateX = v.x;
    _translateY = v.y;
    _translateZ = v.z;
    
    ofPushMatrix();
    
    ofSetColor(_lineColor);
    ofSetLineWidth(_lineWidth);
    ofLine(_translateX, _translateY, _translateZ, _getRawPosition.x, _getRawPosition.y, _getRawPosition.z);
    
    ofTranslate(_translateX, _translateY, _translateZ);
    
    ofRotateX(_rotateX);
    ofRotateY(_rotateY);
    ofRotateZ(_rotateZ);
    
    draw_pan_status();
    draw_tilt_status();
    draw_base_status();
    draw_monitor_status();
    
    if(DEBUG)ofDrawAxis(300);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void MovingHeadObject::draw(float x, float y, float z)
{
    _translateX = x;
    _translateY = y;
    _translateZ = z;
    
    ofPushMatrix();
    
    ofSetColor(_lineColor);
    ofSetLineWidth(_lineWidth);
    ofLine(_translateX, _translateY, _translateZ, _getRawPosition.x, _getRawPosition.y, _getRawPosition.z);
    
    ofTranslate(_translateX, _translateY, _translateZ);
    
    ofRotateX(_rotateX);
    ofRotateY(_rotateY);
    ofRotateZ(_rotateZ);
    
    draw_pan_status();
    draw_tilt_status();
    draw_base_status();
    draw_monitor_status();
    
    if(DEBUG)ofDrawAxis(300);
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void MovingHeadObject::draw_pan_status()
{
    ofPushMatrix();
    
    ofSetColor(_objectColor);
    ofRotateY(_angle_pan);
    _obj_pan->draw();
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void MovingHeadObject::draw_tilt_status()
{
    ofPushMatrix();
    
    ofSetColor(_objectColor);
    ofRotateY(_angle_pan);
    ofRotateZ(_angle_tilt);
    ofTranslate(_obj_tilt->getHeight()/2, 0);
    _obj_tilt->draw();
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void MovingHeadObject::draw_base_status()
{
    ofPushMatrix();
    
    ofSetColor(_objectColor);
    ofTranslate(0, -_obj_pan->getDepth(), 0);
    _obj_base->draw();
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void MovingHeadObject::draw_monitor_status()
{
    ofPushMatrix();
    
    ofSetColor(_monitorColor);
    ofTranslate(0, -_obj_base->getDepth(), -_obj_base->getWidth()/2);
    _obj_monitor->draw();
    
    ofPopMatrix();
}

//--------------------------------------------------------------
float MovingHeadObject::panAngle(ofVec3f v)
{
    ofVec2f re_v1 = ofVec2f(0, 0);
    ofVec2f re_v2 = ofVec2f(v.x, 0);
    ofVec2f re_v3 = ofVec2f(v.x, v.z);
    
    float theta;
    float d1 = re_v1.distance(re_v2);
    float d2 = re_v2.distance(re_v3);
    float d3 = re_v3.distance(re_v1);
    
    theta = (d1*d1 + d3*d3 - d2*d2) / (2 * d1 * d3);
    
    float angle = acos(theta) * (180/PI);
    
    if(re_v3.x == 0)
    {
        angle = 90;
        return angle;
    }
    
    if(re_v3.x >= 0 && re_v3.y <= 0)// area 1
    {
        _preArea = 1;
    }
    else if(re_v3.x <= 0 && re_v3.y <= 0)// area 2
    {
        angle = abs(angle-90) + 90;
        _preArea = 2;
    }
    else if(re_v3.x <= 0 && re_v3.y >= 0)// area 3
    {
        angle = angle + 180;
        _preArea = 3;
    }
    else if(re_v3.x >= 0 && re_v3.y >= 0)// area 4
    {
        angle = abs(angle-90) + 270;
        _preArea = 4;
    }
    
    if(angle >= PAN_MIN && angle <= PAN_MAX)
    {
        return angle;
    }
    else
    {
        return 0;
    }
}

float MovingHeadObject::tiltAngle(ofVec3f v1, ofVec3f v2, ofVec3f v3)
{
    float theta;
    float d1 = v1.distance(v2);
    float d2 = v2.distance(v3);
    float d3 = v3.distance(v1);
    
    theta = (d1*d1 + d3*d3 - d2*d2) / (2 * d1 * d3);
    float angle = acos(theta) * (180/PI);
    
    if(angle >= TILT_MIN && angle <= TILT_MAX)
    {
        return angle;
    }
    else
    {
        return 0;
    }
}
