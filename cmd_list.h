#include "../../security/skCrypter.h"

#define CMD_RETRY skCrypt("retry")

#define CMD_ATTACK_ACTIVATED skCrypt("+attack")
#define CMD_ATTACK_DEACTIVATED skCrypt("-attack")

#define CMD_ATTACK2_ACTIVATED skCrypt("+attack2")
#define CMD_ATTACK2_DEACTIVATED skCrypt("-attack2")

#define CMD_FORWARD_ACTIVATED skCrypt("+forward")
#define CMD_FORWARD_DEACTIVATED skCrypt("-forward")

#define CMD_BACK_ACTIVATED skCrypt("+back")
#define CMD_BACK_DEACTIVATED skCrypt("-back")

#define CMD_MOVELEFT_ACTIVATED skCrypt("+moveleft")
#define CMD_MOVELEFT_DEACTIVATED skCrypt("-moveleft")

#define CMD_MOVERIGHT_ACTIVATED skCrypt("+moveright")
#define CMD_MOVERIGHT_DEACTIVATED skCrypt("-moveright")

#define CMD_JUMP_ACTIVATED skCrypt("+jump")
#define CMD_JUMP_DEACTIVATED skCrypt("-jump")

#define CMD_DUCK_ACTIVATED skCrypt("+duck")
#define CMD_DUCK_DEACTIVATED skCrypt("-duck")

#define CMD_LEFT_ACTIVATED skCrypt("+left")
#define CMD_LEFT_DEACTIVATED skCrypt("-left")

#define CMD_RIGHT_ACTIVATED skCrypt("+right")
#define CMD_RIGHT_DEACTIVATED skCrypt("-right")

#define CMD_IMPULSE100 skCrypt("impulse 100")