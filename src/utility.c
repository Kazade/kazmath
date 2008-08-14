#include "utility.h"

kmScalar kmSQR(kmScalar s) {
	return s*s;
}

kmScalar kmDegreesToRadians(kmScalar degrees) {
	return degrees * kmPIOver180;
}

kmScalar kmRadiansToDegrees(kmScalar radians) {
	return radians * kmPIUnder180;
}

