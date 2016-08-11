# Description

Lua binding for kazmath

## How To New An Object?

```
local vec2 = lkazmath.kmVec2New()
local tbl = vec2:ToTbl()
assert(tbl.x == 0)
assert(tbl.y == 0)

local vec3 = lkazmath.kmVec3NewWithTbl({x=1, y=2, z=3})
local tbl = vec3:ToTbl()
assert(tbl.x == 1)
assert(tbl.y == 2)
assert(tbl.z == 3)

```

## Get or Set Fields As a Table

Implemented for kmVec*, kmRay*, kmPlane, kmQuaternion

for exmaple
```
local vec2 = lkazmath.kmVec2New()
vec2.x = 5
assert(vec2.x == 5)

local ray2 = lkazmath.kmRay2New()
ray2.px = 2
assert(ray2.px == 2)
```

## Method for OOP Style
any function for a object starts with <ObjectTypeName>
has been bind onto the userdata

for example

```
lkazmath.kmMat3Adjugate(mat3, pIn) -> mat3:Adjugate(pIn)
lkazmath.kmPlaneDotCoord(plane, pV) -> plane:DotCoord(pV)
```

```
local aabb2A = lkazmath.kmAABB2New()
aabb2A:Fill(centre1, width1, height1, depth1);

local aabb2B = lkazmath.kmAABB2New()
aabb2B:Fill(centre2, width2, height2, depth2);

aabb2A:ContainsAABB(aabb2, aabb2B)
```


# Macros

```
lkazmath.KM_CONTAINS_NONE
lkazmath.KM_CONTAINS_PARTIAL
lkazmath.KM_CONTAINS_ALL

lkazmath.POINT_BEHIND_PLANE
lkazmath.POINT_ON_PLANE
lkazmath.POINT_INFRONT_OF_PLANE

lkazmath.KM_PLANE_LEFT
lkazmath.KM_PLANE_RIGHT
lkazmath.KM_PLANE_BOTTOM
lkazmath.KM_PLANE_TOP
lkazmath.KM_PLANE_NEAR
lkazmath.KM_PLANE_FAR
```

# Classes

## kmAABB2
### lkazmath.kmAABB2New()


## kmAABB3
### lkazmath.kmAABB3New()


## kmMat3
### lkazmath.kmMat3New()
### lkazmath.kmMat3NewWithArray(array)
### lkazmath.kmMat3ToArray()


## kmMat4
### lkazmath.kmMat4New()
### lkazmath.kmMat4NewWithArray(array)
### lkazmath.kmMat4ToArray()


## kmPlane
### lkazmath.kmPlaneNew()


## kmQuaternion
### lkazmath.kmQuaternionNew()


## kmRay2
### lkazmath.kmRay2New()


## kmRay3
### lkazmath.kmRay3New()


## kmVec2
### lkazmath.kmVec2New()
### lkazmath.kmVec2NewWithArray(array)
### lkazmath.kmVec2NewWithTbl(tbl)
### lkazmath.kmVec2ToArray()
### lkazmath.kmVec2ToTbl()


## kmVec3
### lkazmath.kmVec3New()
### lkazmath.kmVec3NewWithArray(array)
### lkazmath.kmVec3NewWithTbl(tbl)
### lkazmath.kmVec3ToArray()
### lkazmath.kmVec3ToTbl()


## kmVec4
### lkazmath.kmVec4New()
### lkazmath.kmVec4NewWithArray(array)
### lkazmath.kmVec4NewWithTbl(tbl)
### lkazmath.kmVec4ToArray()
### lkazmath.kmVec4ToTbl()



# Util Functions
### lkazmath.kmAlmostEqual(lhs, rhs)
#### returns
boolean
#### params
lhs : number

rhs : number


### lkazmath.kmClamp(x, min, max)
#### returns
number
#### params
x : number

min : number

max : number


### lkazmath.kmDegreesToRadians(degrees)
#### returns
number
#### params
degrees : number


### lkazmath.kmLerp(x, y, factor)
#### returns
number
#### params
x : number

y : number

factor : number


### lkazmath.kmLine2WithLineIntersection(ptA, vecA, ptB, vecB, outIntersection)
```Note : Params Alter```
#### returns
boolean
outTA : number
outTB : number
#### params
ptA : kmVec2

vecA : kmVec2

ptB : kmVec2

vecB : kmVec2

outTA : number

outTB : number

outIntersection : kmVec2


### lkazmath.kmMax(lhs, rhs)
#### returns
number
#### params
lhs : number

rhs : number


### lkazmath.kmMin(lhs, rhs)
#### returns
number
#### params
lhs : number

rhs : number


### lkazmath.kmRadiansToDegrees(radians)
#### returns
number
#### params
radians : number


### lkazmath.kmSQR(s)
#### returns
number
#### params
s : number


### lkazmath.kmSegment2WithSegmentIntersection(segmentA, segmentB, intersection)
#### returns
boolean
#### params
segmentA : kmRay2

segmentB : kmRay2

intersection : kmVec2


# Struct Functions 
### lkazmath.kmAABB2Assign(aabb2, pIn)
#### params
aabb2 : kmAABB2

pIn : kmAABB2


### lkazmath.kmAABB2Centre(aabb2, pOut)
#### params
aabb2 : kmAABB2

pOut : kmVec2


### lkazmath.kmAABB2ContainsAABB(aabb2, to_check)
#### returns
number

```
lkazmath.KM_CONTAINS_NONE
lkazmath.KM_CONTAINS_PARTIAL
lkazmath.KM_CONTAINS_ALL
```

#### params
aabb2 : kmAABB2

to_check : kmAABB2


### lkazmath.kmAABB2ContainsPoint(aabb2, pPoint)
#### returns
number
#### params
aabb2 : kmAABB2

pPoint : kmVec2


### lkazmath.kmAABB2DiameterX(aabb2)
#### returns
number
#### params
aabb2 : kmAABB2


### lkazmath.kmAABB2DiameterY(aabb2)
#### returns
number
#### params
aabb2 : kmAABB2


### lkazmath.kmAABB2ExpandToContain(aabb2, pIn, other)
#### params
aabb2 : kmAABB2

pIn : kmAABB2

other : kmAABB2


### lkazmath.kmAABB2Initialize(aabb2, centre, width, height, depth)
#### params
aabb2 : kmAABB2

centre : kmVec2

width : number

height : number

depth : number


### lkazmath.kmAABB2Sanitize(aabb2, pIn)
#### params
aabb2 : kmAABB2

pIn : kmAABB2


### lkazmath.kmAABB2Scale(aabb2, pIn, s)
#### params
aabb2 : kmAABB2

pIn : kmAABB2

s : number


### lkazmath.kmAABB2ScaleWithPivot(aabb2, pIn, pivot, s)
#### params
aabb2 : kmAABB2

pIn : kmAABB2

pivot : kmVec2

s : number


### lkazmath.kmAABB2Translate(aabb2, pIn, translation)
#### params
aabb2 : kmAABB2

pIn : kmAABB2

translation : kmVec2


### lkazmath.kmAABB3Assign(aabb3, pIn)
#### params
aabb3 : kmAABB3

pIn : kmAABB3


### lkazmath.kmAABB3Centre(aabb3, pOut)
#### params
aabb3 : kmAABB3

pOut : kmVec3


### lkazmath.kmAABB3ContainsAABB(aabb3, to_check)
#### returns
number

```
lkazmath.KM_CONTAINS_NONE
lkazmath.KM_CONTAINS_PARTIAL
lkazmath.KM_CONTAINS_ALL
```

#### params
aabb3 : kmAABB3

to_check : kmAABB3


### lkazmath.kmAABB3ContainsPoint(aabb3, pPoint)
#### returns
number
#### params
aabb3 : kmAABB3

pPoint : kmVec3


### lkazmath.kmAABB3DiameterX(aabb3)
#### returns
number
#### params
aabb3 : kmAABB3


### lkazmath.kmAABB3DiameterY(aabb3)
#### returns
number
#### params
aabb3 : kmAABB3


### lkazmath.kmAABB3DiameterZ(aabb3)
#### returns
number
#### params
aabb3 : kmAABB3


### lkazmath.kmAABB3ExpandToContain(aabb3, pIn, other)
#### params
aabb3 : kmAABB3

pIn : kmAABB3

other : kmAABB3


### lkazmath.kmAABB3Initialize(aabb3, centre, width, height, depth)
#### params
aabb3 : kmAABB3

centre : kmVec3

width : number

height : number

depth : number


### lkazmath.kmAABB3IntersectsAABB(aabb3, other)
#### returns
boolean
#### params
aabb3 : kmAABB3

other : kmAABB3


### lkazmath.kmAABB3IntersectsTriangle(aabb3, p1, p2, p3)
#### returns
boolean
#### params
aabb3 : kmAABB3

p1 : kmVec3

p2 : kmVec3

p3 : kmVec3


### lkazmath.kmAABB3Scale(aabb3, pIn, s)
#### params
aabb3 : kmAABB3

pIn : kmAABB3

s : number


### lkazmath.kmMat3Adjugate(mat3, pIn)
#### params
mat3 : kmMat3

pIn : kmMat3


### lkazmath.kmMat3AreEqual(mat3, pMat2)
#### returns
boolean
#### params
mat3 : kmMat3

pMat2 : kmMat3


### lkazmath.kmMat3AssignMat3(mat3, pIn)
#### params
mat3 : kmMat3

pIn : kmMat3


### lkazmath.kmMat3Determinant(mat3)
#### returns
number
#### params
mat3 : kmMat3


### lkazmath.kmMat3ExtractForwardVec3(mat3, pOut)
#### params
mat3 : kmMat3

pOut : kmVec3


### lkazmath.kmMat3ExtractRightVec3(mat3, pOut)
#### params
mat3 : kmMat3

pOut : kmVec3


### lkazmath.kmMat3ExtractRotationAxisAngle(mat3, axis, radians)
#### params
mat3 : kmMat3

axis : kmVec3

radians : number


### lkazmath.kmMat3ExtractRotationAxisAngleInDegrees(mat3, axis, degrees)
#### params
mat3 : kmMat3

axis : kmVec3

degrees : number


### lkazmath.kmMat3ExtractUpVec3(mat3, pOut)
#### params
mat3 : kmMat3

pOut : kmVec3


### lkazmath.kmMat3Fill(mat3, pMat)
#### params
mat3 : kmMat3

pMat : number


### lkazmath.kmMat3FromRotationAxisAngle(mat3, axis, radians)
#### params
mat3 : kmMat3

axis : kmVec3

radians : number


### lkazmath.kmMat3FromRotationAxisAngleInDegrees(mat3, axis, degrees)
#### params
mat3 : kmMat3

axis : kmVec3

degrees : number


### lkazmath.kmMat3FromRotationLookAt(mat3, pEye, pCentre, pUp)
#### params
mat3 : kmMat3

pEye : kmVec3

pCentre : kmVec3

pUp : kmVec3


### lkazmath.kmMat3FromRotationQuaternion(mat3, quaternion)
#### params
mat3 : kmMat3

quaternion : kmQuaternion


### lkazmath.kmMat3FromRotationX(mat3, radians)
#### params
mat3 : kmMat3

radians : number


### lkazmath.kmMat3FromRotationXInDegrees(mat3, degrees)
#### params
mat3 : kmMat3

degrees : number


### lkazmath.kmMat3FromRotationY(mat3, radians)
#### params
mat3 : kmMat3

radians : number


### lkazmath.kmMat3FromRotationYInDegrees(mat3, degrees)
#### params
mat3 : kmMat3

degrees : number


### lkazmath.kmMat3FromRotationZ(mat3, radians)
#### params
mat3 : kmMat3

radians : number


### lkazmath.kmMat3FromRotationZInDegrees(mat3, degrees)
#### params
mat3 : kmMat3

degrees : number


### lkazmath.kmMat3FromScaling(mat3, x, y)
#### params
mat3 : kmMat3

x : number

y : number


### lkazmath.kmMat3FromTranslation(mat3, x, y)
#### params
mat3 : kmMat3

x : number

y : number


### lkazmath.kmMat3Identity(mat3)
#### params
mat3 : kmMat3


### lkazmath.kmMat3Inverse(mat3, pM)
#### params
mat3 : kmMat3

pM : kmMat3


### lkazmath.kmMat3IsIdentity(mat3)
#### returns
boolean
#### params
mat3 : kmMat3


### lkazmath.kmMat3MultiplyMat3(mat3, lhs, rhs)
#### params
mat3 : kmMat3

lhs : kmMat3

rhs : kmMat3


### lkazmath.kmMat3MultiplyScalar(mat3, lhs, rhs)
#### params
mat3 : kmMat3

lhs : kmMat3

rhs : number


### lkazmath.kmMat3Transpose(mat3, pIn)
#### params
mat3 : kmMat3

pIn : kmMat3


### lkazmath.kmMat4AreEqual(mat4, pM2)
#### returns
number
#### params
mat4 : kmMat4

pM2 : kmMat4


### lkazmath.kmMat4Assign(mat4, pIn)
#### params
mat4 : kmMat4

pIn : kmMat4


### lkazmath.kmMat4AssignMat3(mat4, pIn)
#### params
mat4 : kmMat4

pIn : kmMat3


### lkazmath.kmMat4ExtractPlane(mat4, pIn, plane)
#### params
mat4 : kmPlane

pIn : kmMat4

plane : number 

```
lkazmath.KM_PLANE_LEFT
lkazmath.KM_PLANE_RIGHT
lkazmath.KM_PLANE_BOTTOM
lkazmath.KM_PLANE_TOP
lkazmath.KM_PLANE_NEAR
lkazmath.KM_PLANE_FAR
```


### lkazmath.kmMat4ExtractRotationMat3(mat4, pOut)
#### params
mat4 : kmMat4

pOut : kmMat3


### lkazmath.kmMat4ExtractTranslationVec3(mat4, pOut)
#### params
mat4 : kmMat4

pOut : kmVec3


### lkazmath.kmMat4Fill(mat4, pMat)
#### params
mat4 : kmMat4

pMat : number


### lkazmath.kmMat4GetForwardVec3LH(mat4, pIn)
#### params
mat4 : kmVec3

pIn : kmMat4


### lkazmath.kmMat4GetForwardVec3RH(mat4, pIn)
#### params
mat4 : kmVec3

pIn : kmMat4


### lkazmath.kmMat4GetRightVec3(mat4, pIn)
#### params
mat4 : kmVec3

pIn : kmMat4


### lkazmath.kmMat4GetUpVec3(mat4, pIn)
#### params
mat4 : kmVec3

pIn : kmMat4


### lkazmath.kmMat4Identity(mat4)
#### params
mat4 : kmMat4


### lkazmath.kmMat4Inverse(mat4, pM)
#### params
mat4 : kmMat4

pM : kmMat4


### lkazmath.kmMat4IsIdentity(mat4)
#### returns
number
#### params
mat4 : kmMat4


### lkazmath.kmMat4LookAt(mat4, pEye, pCenter, pUp)
#### params
mat4 : kmMat4

pEye : kmVec3

pCenter : kmVec3

pUp : kmVec3


### lkazmath.kmMat4Multiply(mat4, pM1, pM2)
#### params
mat4 : kmMat4

pM1 : kmMat4

pM2 : kmMat4


### lkazmath.kmMat4OrthographicProjection(mat4, left, right, bottom, top, nearVal, farVal)
#### params
mat4 : kmMat4

left : number

right : number

bottom : number

top : number

nearVal : number

farVal : number


### lkazmath.kmMat4PerspectiveProjection(mat4, fovY, aspect, zNear, zFar)
#### params
mat4 : kmMat4

fovY : number

aspect : number

zNear : number

zFar : number


### lkazmath.kmMat4RotationAxisAngle(mat4, axis, radians)
#### params
mat4 : kmMat4

axis : kmVec3

radians : number


### lkazmath.kmMat4RotationQuaternion(mat4, pQ)
#### params
mat4 : kmMat4

pQ : kmQuaternion


### lkazmath.kmMat4RotationToAxisAngle(mat4, pIn)
```Note : Params Alter```
#### returns
radians : number
#### params
mat4 : kmVec3

radians : number

pIn : kmMat4


### lkazmath.kmMat4RotationTranslation(mat4, rotation, translation)
#### params
mat4 : kmMat4

rotation : kmMat3

translation : kmVec3


### lkazmath.kmMat4RotationX(mat4, radians)
#### params
mat4 : kmMat4

radians : number


### lkazmath.kmMat4RotationY(mat4, radians)
#### params
mat4 : kmMat4

radians : number


### lkazmath.kmMat4RotationYawPitchRoll(mat4, pitch, yaw, roll)
#### params
mat4 : kmMat4

pitch : number

yaw : number

roll : number


### lkazmath.kmMat4RotationZ(mat4, radians)
#### params
mat4 : kmMat4

radians : number


### lkazmath.kmMat4Scaling(mat4, x, y, z)
#### params
mat4 : kmMat4

x : number

y : number

z : number


### lkazmath.kmMat4Translation(mat4, x, y, z)
#### params
mat4 : kmMat4

x : number

y : number

z : number


### lkazmath.kmMat4Transpose(mat4, pIn)
#### params
mat4 : kmMat4

pIn : kmMat4


### lkazmath.kmPlaneClassifyPoint(plane, pP)
#### returns
number 

```
lkazmath.POINT_BEHIND_PLANE
lkazmath.POINT_ON_PLANE
lkazmath.POINT_INFRONT_OF_PLANE
```

#### params
plane : kmPlane

pP : kmVec3


### lkazmath.kmPlaneDot(plane, pV)
#### returns
number
#### params
plane : kmPlane

pV : kmVec4


### lkazmath.kmPlaneDotCoord(plane, pV)
#### returns
number
#### params
plane : kmPlane

pV : kmVec3


### lkazmath.kmPlaneDotNormal(plane, pV)
#### returns
number
#### params
plane : kmPlane

pV : kmVec3


### lkazmath.kmPlaneExtractFromMat4(plane, pIn, row)
#### params
plane : kmPlane

pIn : kmMat4

row : kmInt


### lkazmath.kmPlaneFill(plane, a, b, c, d)
#### params
plane : kmPlane

a : number

b : number

c : number

d : number


### lkazmath.kmPlaneFromNormalAndDistance(plane, normal, dist)
#### params
plane : kmPlane

normal : kmVec3

dist : number


### lkazmath.kmPlaneFromPointAndNormal(plane, pPoint, pNormal)
#### params
plane : kmPlane

pPoint : kmVec3

pNormal : kmVec3


### lkazmath.kmPlaneFromPoints(plane, p1, p2, p3)
#### params
plane : kmPlane

p1 : kmVec3

p2 : kmVec3

p3 : kmVec3


### lkazmath.kmPlaneGetIntersection(plane, p1, p2, p3)
#### params
plane : kmVec3

p1 : kmPlane

p2 : kmPlane

p3 : kmPlane


### lkazmath.kmPlaneIntersectLine(plane, pP, pV1, pV2)
#### params
plane : kmVec3

pP : kmPlane

pV1 : kmVec3

pV2 : kmVec3


### lkazmath.kmPlaneNormalize(plane, pP)
#### params
plane : kmPlane

pP : kmPlane


### lkazmath.kmPlaneScale(plane, pP, s)
#### params
plane : kmPlane

pP : kmPlane

s : number


### lkazmath.kmQuaternionAdd(quaternion, pQ1, pQ2)
#### params
quaternion : kmQuaternion

pQ1 : kmQuaternion

pQ2 : kmQuaternion


### lkazmath.kmQuaternionAreEqual(quaternion, p2)
#### returns
number
#### params
quaternion : kmQuaternion

p2 : kmQuaternion


### lkazmath.kmQuaternionAssign(quaternion, pIn)
#### params
quaternion : kmQuaternion

pIn : kmQuaternion


### lkazmath.kmQuaternionBetweenVec3(quaternion, v1, v2)
#### params
quaternion : kmQuaternion

v1 : kmVec3

v2 : kmVec3


### lkazmath.kmQuaternionDot(quaternion, q2)
#### returns
number
#### params
quaternion : kmQuaternion

q2 : kmQuaternion


### lkazmath.kmQuaternionExp(quaternion, pIn)
#### params
quaternion : kmQuaternion

pIn : kmQuaternion


### lkazmath.kmQuaternionExtractRotationAroundAxis(quaternion, axis, pOut)
#### params
quaternion : kmQuaternion

axis : kmVec3

pOut : kmQuaternion


### lkazmath.kmQuaternionFill(quaternion, x, y, z, w)
#### params
quaternion : kmQuaternion

x : number

y : number

z : number

w : number


### lkazmath.kmQuaternionGetForwardVec3LH(quaternion, pIn)
#### params
quaternion : kmVec3

pIn : kmQuaternion


### lkazmath.kmQuaternionGetForwardVec3RH(quaternion, pIn)
#### params
quaternion : kmVec3

pIn : kmQuaternion


### lkazmath.kmQuaternionGetPitch(quaternion)
#### returns
number
#### params
quaternion : kmQuaternion


### lkazmath.kmQuaternionGetRightVec3(quaternion, pIn)
#### params
quaternion : kmVec3

pIn : kmQuaternion


### lkazmath.kmQuaternionGetRoll(quaternion)
#### returns
number
#### params
quaternion : kmQuaternion


### lkazmath.kmQuaternionGetUpVec3(quaternion, pIn)
#### params
quaternion : kmVec3

pIn : kmQuaternion


### lkazmath.kmQuaternionGetYaw(quaternion)
#### returns
number
#### params
quaternion : kmQuaternion


### lkazmath.kmQuaternionIdentity(quaternion)
#### params
quaternion : kmQuaternion


### lkazmath.kmQuaternionInverse(quaternion, pIn)
#### params
quaternion : kmQuaternion

pIn : kmQuaternion


### lkazmath.kmQuaternionIsIdentity(quaternion)
#### returns
number
#### params
quaternion : kmQuaternion


### lkazmath.kmQuaternionLength(quaternion)
#### returns
number
#### params
quaternion : kmQuaternion


### lkazmath.kmQuaternionLengthSq(quaternion)
#### returns
number
#### params
quaternion : kmQuaternion


### lkazmath.kmQuaternionLn(quaternion, pIn)
#### params
quaternion : kmQuaternion

pIn : kmQuaternion


### lkazmath.kmQuaternionLookRotation(quaternion, direction, up)
#### params
quaternion : kmQuaternion

direction : kmVec3

up : kmVec3


### lkazmath.kmQuaternionMultiply(quaternion, q1, q2)
#### params
quaternion : kmQuaternion

q1 : kmQuaternion

q2 : kmQuaternion


### lkazmath.kmQuaternionMultiplyVec3(quaternion, q, v)
#### params
quaternion : kmVec3

q : kmQuaternion

v : kmVec3


### lkazmath.kmQuaternionNormalize(quaternion, pIn)
#### params
quaternion : kmQuaternion

pIn : kmQuaternion


### lkazmath.kmQuaternionRotationAxisAngle(quaternion, pV, angle)
#### params
quaternion : kmQuaternion

pV : kmVec3

angle : number


### lkazmath.kmQuaternionRotationBetweenVec3(quaternion, vec1, vec2, fallback)
#### params
quaternion : kmQuaternion

vec1 : kmVec3

vec2 : kmVec3

fallback : kmVec3


### lkazmath.kmQuaternionRotationMatrix(quaternion, pIn)
#### params
quaternion : kmQuaternion

pIn : kmMat3


### lkazmath.kmQuaternionRotationPitchYawRoll(quaternion, pitch, yaw, roll)
#### params
quaternion : kmQuaternion

pitch : number

yaw : number

roll : number


### lkazmath.kmQuaternionScale(quaternion, pIn, s)
#### params
quaternion : kmQuaternion

pIn : kmQuaternion

s : number


### lkazmath.kmQuaternionSlerp(quaternion, q1, q2, t)
#### params
quaternion : kmQuaternion

q1 : kmQuaternion

q2 : kmQuaternion

t : number


### lkazmath.kmQuaternionSubtract(quaternion, pQ1, pQ2)
#### params
quaternion : kmQuaternion

pQ1 : kmQuaternion

pQ2 : kmQuaternion


### lkazmath.kmQuaternionToAxisAngle(quaternion, pVector, pAngle)
#### params
quaternion : kmQuaternion

pVector : kmVec3

pAngle : number


### lkazmath.kmRay2Fill(ray2, px, py, vx, vy)
#### params
ray2 : kmRay2

px : number

py : number

vx : number

vy : number


### lkazmath.kmRay2FillWithEndpoints(ray2, start, end)
#### params
ray2 : kmRay2

start : kmVec2

end : kmVec2


### lkazmath.kmRay2IntersectBox(ray2, p1, p2, p3, p4, intersection, normal_out)
#### returns
boolean
#### params
ray2 : kmRay2

p1 : kmVec2

p2 : kmVec2

p3 : kmVec2

p4 : kmVec2

intersection : kmVec2

normal_out : kmVec2


### lkazmath.kmRay2IntersectCircle(ray2, centre, radius, intersection)
#### returns
boolean
#### params
ray2 : kmRay2

centre : kmVec2

radius : number

intersection : kmVec2


### lkazmath.kmRay2IntersectLineSegment(ray2, p1, p2, intersection)
#### returns
boolean
#### params
ray2 : kmRay2

p1 : kmVec2

p2 : kmVec2

intersection : kmVec2


### lkazmath.kmRay2IntersectTriangle(ray2, p1, p2, p3, intersection, normal_out)
#### returns
boolean

distance : number
#### params
ray2 : kmRay2

p1 : kmVec2

p2 : kmVec2

p3 : kmVec2

intersection : kmVec2

normal_out : kmVec2


### lkazmath.kmRay3Fill(ray3, px, py, pz, vx, vy, vz)
#### params
ray3 : kmRay3

px : number

py : number

pz : number

vx : number

vy : number

vz : number


### lkazmath.kmRay3FromPointAndDirection(ray3, point, direction)
#### params
ray3 : kmRay3

point : kmVec3

direction : kmVec3


### lkazmath.kmRay3IntersectPlane(ray3, ray, plane)
#### returns
boolean
#### params
ray3 : kmVec3

ray : kmRay3

plane : kmPlane


### lkazmath.kmRay3IntersectTriangle(ray3, v0, v1, v2, intersection, normal)
#### returns
boolean

distance : number
#### params
ray3 : kmRay3

v0 : kmVec3

v1 : kmVec3

v2 : kmVec3

intersection : kmVec3

normal : kmVec3


### lkazmath.kmVec2Add(vec2, pV1, pV2)
#### params
vec2 : kmVec2

pV1 : kmVec2

pV2 : kmVec2


### lkazmath.kmVec2AreEqual(vec2, p2)
#### returns
number
#### params
vec2 : kmVec2

p2 : kmVec2


### lkazmath.kmVec2Assign(vec2, pIn)
#### params
vec2 : kmVec2

pIn : kmVec2


### lkazmath.kmVec2Cross(vec2, pV2)
#### returns
number
#### params
vec2 : kmVec2

pV2 : kmVec2


### lkazmath.kmVec2DegreesBetween(vec2, v2)
#### returns
number
#### params
vec2 : kmVec2

v2 : kmVec2


### lkazmath.kmVec2DistanceBetween(vec2, v2)
#### returns
number
#### params
vec2 : kmVec2

v2 : kmVec2


### lkazmath.kmVec2Div(vec2, pV1, pV2)
#### params
vec2 : kmVec2

pV1 : kmVec2

pV2 : kmVec2


### lkazmath.kmVec2Dot(vec2, pV2)
#### returns
number
#### params
vec2 : kmVec2

pV2 : kmVec2


### lkazmath.kmVec2Fill(vec2, x, y)
#### params
vec2 : kmVec2

x : number

y : number


### lkazmath.kmVec2Length(vec2)
#### returns
number
#### params
vec2 : kmVec2


### lkazmath.kmVec2LengthSq(vec2)
#### returns
number
#### params
vec2 : kmVec2


### lkazmath.kmVec2Lerp(vec2, pV1, pV2, t)
#### params
vec2 : kmVec2

pV1 : kmVec2

pV2 : kmVec2

t : number


### lkazmath.kmVec2MidPointBetween(vec2, v1, v2)
#### params
vec2 : kmVec2

v1 : kmVec2

v2 : kmVec2


### lkazmath.kmVec2Mul(vec2, pV1, pV2)
#### params
vec2 : kmVec2

pV1 : kmVec2

pV2 : kmVec2


### lkazmath.kmVec2Normalize(vec2, pIn)
#### params
vec2 : kmVec2

pIn : kmVec2


### lkazmath.kmVec2Reflect(vec2, pIn, normal)
#### params
vec2 : kmVec2

pIn : kmVec2

normal : kmVec2


### lkazmath.kmVec2RotateBy(vec2, pIn, degrees, center)
#### params
vec2 : kmVec2

pIn : kmVec2

degrees : number

center : kmVec2


### lkazmath.kmVec2Scale(vec2, pIn, s)
#### params
vec2 : kmVec2

pIn : kmVec2

s : number


### lkazmath.kmVec2Subtract(vec2, pV1, pV2)
#### params
vec2 : kmVec2

pV1 : kmVec2

pV2 : kmVec2


### lkazmath.kmVec2Swap(vec2, pB)
#### params
vec2 : kmVec2

pB : kmVec2


### lkazmath.kmVec2Transform(vec2, pV1, pM)
#### params
vec2 : kmVec2

pV1 : kmVec2

pM : kmMat3


### lkazmath.kmVec2TransformCoord(vec2, pV, pM)
#### params
vec2 : kmVec2

pV : kmVec2

pM : kmMat3


### lkazmath.kmVec3Add(vec3, pV1, pV2)
#### params
vec3 : kmVec3

pV1 : kmVec3

pV2 : kmVec3


### lkazmath.kmVec3AreEqual(vec3, p2)
#### returns
boolean
#### params
vec3 : kmVec3

p2 : kmVec3


### lkazmath.kmVec3Assign(vec3, pIn)
#### params
vec3 : kmVec3

pIn : kmVec3


### lkazmath.kmVec3Cross(vec3, pV1, pV2)
#### params
vec3 : kmVec3

pV1 : kmVec3

pV2 : kmVec3


### lkazmath.kmVec3Div(vec3, pV1, pV2)
#### params
vec3 : kmVec3

pV1 : kmVec3

pV2 : kmVec3


### lkazmath.kmVec3Dot(vec3, pV2)
#### returns
number
#### params
vec3 : kmVec3

pV2 : kmVec3


### lkazmath.kmVec3Fill(vec3, x, y, z)
#### params
vec3 : kmVec3

x : number

y : number

z : number


### lkazmath.kmVec3GetHorizontalAngle(vec3, pIn)
#### params
vec3 : kmVec3

pIn : kmVec3


### lkazmath.kmVec3InverseTransform(vec3, pV, pM)
#### params
vec3 : kmVec3

pV : kmVec3

pM : kmMat4


### lkazmath.kmVec3InverseTransformNormal(vec3, pVect, pM)
#### params
vec3 : kmVec3

pVect : kmVec3

pM : kmMat4


### lkazmath.kmVec3Length(vec3)
#### returns
number
#### params
vec3 : kmVec3


### lkazmath.kmVec3LengthSq(vec3)
#### returns
number
#### params
vec3 : kmVec3


### lkazmath.kmVec3Lerp(vec3, pV1, pV2, t)
#### params
vec3 : kmVec3

pV1 : kmVec3

pV2 : kmVec3

t : number


### lkazmath.kmVec3Mul(vec3, pV1, pV2)
#### params
vec3 : kmVec3

pV1 : kmVec3

pV2 : kmVec3


### lkazmath.kmVec3MultiplyMat3(vec3, pV, pM)
#### params
vec3 : kmVec3

pV : kmVec3

pM : kmMat3


### lkazmath.kmVec3MultiplyMat4(vec3, pV, pM)
#### params
vec3 : kmVec3

pV : kmVec3

pM : kmMat4


### lkazmath.kmVec3Normalize(vec3, pIn)
#### params
vec3 : kmVec3

pIn : kmVec3


### lkazmath.kmVec3OrthoNormalize(vec3, tangent)
#### params
vec3 : kmVec3

tangent : kmVec3


### lkazmath.kmVec3ProjectOnToPlane(vec3, point, plane)
#### params
vec3 : kmVec3

point : kmVec3

plane : kmPlane


### lkazmath.kmVec3ProjectOnToVec3(vec3, other, projection)
#### params
vec3 : kmVec3

other : kmVec3

projection : kmVec3


### lkazmath.kmVec3Reflect(vec3, pIn, normal)
#### params
vec3 : kmVec3

pIn : kmVec3

normal : kmVec3


### lkazmath.kmVec3RotationToDirection(vec3, pIn, forwards)
#### params
vec3 : kmVec3

pIn : kmVec3

forwards : kmVec3


### lkazmath.kmVec3Scale(vec3, pIn, s)
#### params
vec3 : kmVec3

pIn : kmVec3

s : number


### lkazmath.kmVec3Subtract(vec3, pV1, pV2)
#### params
vec3 : kmVec3

pV1 : kmVec3

pV2 : kmVec3


### lkazmath.kmVec3Swap(vec3, b)
#### params
vec3 : kmVec3

b : kmVec3


### lkazmath.kmVec3Transform(vec3, pV1, pM)
#### params
vec3 : kmVec3

pV1 : kmVec3

pM : kmMat4


### lkazmath.kmVec3TransformCoord(vec3, pV, pM)
#### params
vec3 : kmVec3

pV : kmVec3

pM : kmMat4


### lkazmath.kmVec3TransformNormal(vec3, pV, pM)
#### params
vec3 : kmVec3

pV : kmVec3

pM : kmMat4


### lkazmath.kmVec3Zero(vec3)
#### params
vec3 : kmVec3


### lkazmath.kmVec4Add(vec4, pV1, pV2)
#### params
vec4 : kmVec4

pV1 : kmVec4

pV2 : kmVec4


### lkazmath.kmVec4AreEqual(vec4, p2)
#### returns
number
#### params
vec4 : kmVec4

p2 : kmVec4


### lkazmath.kmVec4Assign(vec4, pIn)
#### params
vec4 : kmVec4

pIn : kmVec4


### lkazmath.kmVec4Div(vec4, pV1, pV2)
#### params
vec4 : kmVec4

pV1 : kmVec4

pV2 : kmVec4


### lkazmath.kmVec4Dot(vec4, pV2)
#### returns
number
#### params
vec4 : kmVec4

pV2 : kmVec4


### lkazmath.kmVec4Fill(vec4, x, y, z, w)
#### params
vec4 : kmVec4

x : number

y : number

z : number

w : number


### lkazmath.kmVec4Length(vec4)
#### returns
number
#### params
vec4 : kmVec4


### lkazmath.kmVec4LengthSq(vec4)
#### returns
number
#### params
vec4 : kmVec4


### lkazmath.kmVec4Lerp(vec4, pV1, pV2, t)
#### params
vec4 : kmVec4

pV1 : kmVec4

pV2 : kmVec4

t : number


### lkazmath.kmVec4Mul(vec4, pV1, pV2)
#### params
vec4 : kmVec4

pV1 : kmVec4

pV2 : kmVec4


### lkazmath.kmVec4MultiplyMat4(vec4, pV, pM)
#### params
vec4 : kmVec4

pV : kmVec4

pM : kmMat4


### lkazmath.kmVec4Normalize(vec4, pIn)
#### params
vec4 : kmVec4

pIn : kmVec4


### lkazmath.kmVec4Scale(vec4, pIn, s)
#### params
vec4 : kmVec4

pIn : kmVec4

s : number


### lkazmath.kmVec4Subtract(vec4, pV1, pV2)
#### params
vec4 : kmVec4

pV1 : kmVec4

pV2 : kmVec4


### lkazmath.kmVec4Swap(vec4, pB)
#### params
vec4 : kmVec4

pB : kmVec4


### lkazmath.kmVec4Transform(vec4, pV, pM)
#### params
vec4 : kmVec4

pV : kmVec4

pM : kmMat4


### lkazmath.kmVec4TransformArray(vec4, outStride, pV, vStride, pM, count)
#### params
vec4 : kmVec4

outStride : unsigned int

pV : kmVec4

vStride : unsigned int

pM : kmMat4

count : unsigned int


