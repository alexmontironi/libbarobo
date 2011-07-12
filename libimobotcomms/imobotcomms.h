#ifndef _IMOBOTCOMMS_H_
#define _IMOBOTCOMMS_H_

#ifndef _WIN32
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#else
//#include <types.h>
#include <winsock2.h>
#include <Ws2bth.h>
#endif

#ifndef _WIN32
typedef struct sockaddr_rc sockaddr_t;
#else
#define AF_BLUETOOTH AF_BTH
#define BTPROTO_RFCOMM BTHPROTO_RFCOMM
typedef SOCKADDR_BTH sockaddr_t;
typedef unsigned char uint8_t;
#endif

#define DEGREE2ENC(angles, enc) \
enc[0] = angles[0] * 10; \
enc[1] = angles[1] * 10; \
enc[2] = angles[2] * 10; \
enc[3] = angles[3] * 10

typedef struct br_comms_s
{
  sockaddr_t addr;
  int socket;
  int connected;
} br_comms_t;

#ifdef __cplusplus
extern "C" {
#endif

int iMobotComms_init(br_comms_t* comms);
int iMobotComms_connect(br_comms_t* comms, const char* address, int channel);
int iMobotComms_disconnect(br_comms_t* comms);
int iMobotComms_isConnected(br_comms_t* comms);
int iMobotComms_setMotorDirection(br_comms_t* comms, int id, int dir);
int iMobotComms_getMotorDirection(br_comms_t* comms, int id, int *dir);
int iMobotComms_setMotorSpeed(br_comms_t* comms, int id, int speed);
int iMobotComms_getMotorSpeed(br_comms_t* comms, int id, int *speed);
int iMobotComms_setMotorPosition(br_comms_t* comms, int id, double position);
int iMobotComms_getMotorPosition(br_comms_t* comms, int id, double *position);
int iMobotComms_getMotorState(br_comms_t* comms, int id, int *state);
int iMobotComms_waitMotor(br_comms_t* comms, int id);
int iMobotComms_stop(br_comms_t* comms);
#ifdef _WIN32
typedef struct bdaddr_s {
  UINT8 b[6];
} bdaddr_t;
int str2ba(const char *str, bdaddr_t *ba);
#define write(sock, buf, len) send(sock, buf, len, 0)
#define read(sock, buf, len) \
  recvfrom(sock, buf, len, 0, (struct sockaddr*)0, 0)
void baswap(bdaddr_t *dst, const bdaddr_t *src);
int str2ba(const char *str, bdaddr_t *ba);
#endif

#ifdef __cplusplus
}
#endif

#if defined (__cplusplus) || defined (_CH_)
class CiMobotComms {
  public:
    CiMobotComms();
    ~CiMobotComms();
    int connect(const char* address, int channel);
    int disconnect();
    int isConnected();
    int setMotorDirection(int id, int dir);
    int getMotorDirection(int id, int &dir);
    int setMotorSpeed(int id, int speed);
    int getMotorSpeed(int id, int &speed);
    int setMotorPosition(int id, double position);
    int getMotorPosition(int id, double &position);
    int getMotorState(int id, int &state);
    int waitMotor(int id);
    int stop();
  private:
    br_comms_t _comms;
};
#endif

#endif
