/**
 * The MIT License (MIT)
 * 
 * Copyright (c) 2016 Roc<RocAltair@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <lua.h>
#include <lauxlib.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <kazmath.h>
#include <utility.h>
#include <aabb2.h>
#include <aabb3.h>
#include <mat3.h>
#include <mat4.h>
#include <plane.h>
#include <quaternion.h>
#include <ray2.h>
#include <ray3.h>
#include <vec2.h>
#include <vec3.h>
#include <vec4.h>

#if (LUA_VERSION_NUM < 502 && !defined(luaL_newlib))
#  define luaL_newlib(L,l) (lua_newtable(L), luaL_register(L,NULL,l))
#endif

#define KAZMATH_CHECK_ARRAY_LEN(L, idx, rqlen) do {                                \
	size_t len = 0;                                                            \
	int isarray = luac__is_array(L, idx);                                      \
	if (isarray) {                                                             \
	        len = lua_objlen(L, idx);                                          \
	}                                                                          \
	if (!isarray || len != rqlen) {                                            \
	        return luaL_error(L, "#%d array(len=%d/%d) required in %s",        \
	                          idx, len, rqlen, __FUNCTION__);                  \
	}                                                                          \
} while(0)

#define KAZMATH_FILL_ARRAY(L, arridx, array, type, tofunc) do {     \
	int idx = 0;                                                \
	lua_pushnil(L);                                             \
	while (lua_next(L, arridx) != 0) {                          \
	        array[idx] = (type)tofunc(L, -1);                   \
	        idx++;                                              \
	        lua_pop(L, 1);                                      \
	}                                                           \
} while(0)

#define KAZMATH_LUA_SETMACRO(L, index, name, var)        \
	(lua_pushstring(L, name),                        \
	lua_pushinteger(L, var),                         \
	lua_settable(L, index >= 0 ? index : index - 2))

#define KAZMATH_LUA_BIND_META(L, type_t, ptr, mname) do {           \
	type_t **my__p = lua_newuserdata(L, sizeof(void *));        \
	*my__p = ptr;                                               \
	luaL_getmetatable(L, mname);                                \
	lua_setmetatable(L, -2);                                    \
} while(0)


#define KAZMATH_EXTEND_NEW_NUMBER_FIELD(fname, rfname)        \
	if (strcmp(fieldname, #fname) == 0) {                 \
	        ptr->rfname = value;                          \
	}

#define KAZMATH_LUA_NEWINDEX_FILED(L, FIELD_MAP, ptr)                 \
	size_t sz;                                                    \
	const char * fieldname = luaL_checklstring(L, 2, &sz);        \
	lua_Number value = luaL_checknumber(L, 3);                    \
	FIELD_MAP(KAZMATH_EXTEND_NEW_NUMBER_FIELD);


#define KAZMATH_LUA_INDEX_METHOD(L, CLS_NAME) do {                                      \
	int top = lua_gettop(L);                                                        \
	size_t sz = 0;                                                                  \
	const char *fieldname = NULL;                                                   \
	if (lua_type(L, 2) != LUA_TSTRING) {                                            \
	        break;                                                                  \
	}                                                                               \
	fieldname = lua_tolstring(L, 2, &sz);                                           \
	luaL_getmetatable(L, CLS_NAME);                                                 \
	if (lua_type(L, -1) != LUA_TTABLE) {                                            \
	        return luaL_error(L, "metatable for %s not found!", CLS_NAME);          \
	}                                                                               \
	lua_pushstring(L, "__method");                                                  \
	lua_rawget(L, -2);                                                              \
	if (lua_type(L, -1) != LUA_TTABLE) {                                            \
	        return luaL_error(L,                                                    \
	                          "metatable field .__method for %s not found!",        \
	                          CLS_NAME);                                            \
	}                                                                               \
	lua_pushlstring(L, fieldname, sz);                                              \
	lua_rawget(L, -2);                                                              \
	                                                                                \
	if (!lua_isnoneornil(L, -1)) {                                                  \
	        return 1;                                                               \
	}                                                                               \
	lua_settop(L, top);                                                             \
} while(0);

#define KAZMATH_EXTEND_NUMBER_FIELD(fname, rfname)                    \
	if (strcmp(fieldname, #fname) == 0) {                         \
	        lua_pushnumber(L, (lua_Number)((ptr)->rfname));       \
	        return 1;                                             \
	}

#define KAZMATH_LUA_INDEX_FIELD(L, FIELD_MAP, ptr) do {                   \
	int type = lua_type(L, 2);                                        \
	if (type == LUA_TSTRING) {                                        \
	        size_t sz;                                                \
	        const char * fieldname = lua_tolstring(L, 2, &sz);        \
	        FIELD_MAP(KAZMATH_EXTEND_NUMBER_FIELD);                   \
	}                                                                 \
} while(0);
	
#define KAZMATH_LUA_INDEX_ARRAY(L, maxIdx, ptr, nFieldName) do {           \
	int type = lua_type(L, 2);                                         \
	if (type == LUA_TNUMBER) {                                         \
	        int cond;                                                  \
	        int fieldIdx = lua_tointeger(L, 2);                        \
	        cond = fieldIdx >=0 && fieldIdx < maxIdx;                  \
	        luaL_argcheck(L, cond, 2, "index out of range");           \
	        lua_pushnumber(L, ptr->nFieldName[fieldIdx]);              \
	        return 1;                                                  \
	}                                                                  \
} while(0);
	
#define KAZMATH_CLS_KMMAT3 "KAZMATH{kmMat3}"
#define KAZMATH_CLS_KMRAY2 "KAZMATH{kmRay2}"
#define KAZMATH_CLS_KMRAY3 "KAZMATH{kmRay3}"
#define KAZMATH_CLS_KMQUATERNION "KAZMATH{kmQuaternion}"
#define KAZMATH_CLS_KMMAT4 "KAZMATH{kmMat4}"
#define KAZMATH_CLS_KMAABB2 "KAZMATH{kmAABB2}"
#define KAZMATH_CLS_KMPLANE "KAZMATH{kmPlane}"
#define KAZMATH_CLS_KMVEC2 "KAZMATH{kmVec2}"
#define KAZMATH_CLS_KMVEC3 "KAZMATH{kmVec3}"
#define KAZMATH_CLS_KMVEC4 "KAZMATH{kmVec4}"
#define KAZMATH_CLS_KMAABB3 "KAZMATH{kmAABB3}"

#define KAZMATH_CHECK_KMMAT3(L, idx) (*(kmMat3 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMMAT3))
#define KAZMATH_CHECK_KMRAY2(L, idx) (*(kmRay2 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMRAY2))
#define KAZMATH_CHECK_KMRAY3(L, idx) (*(kmRay3 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMRAY3))
#define KAZMATH_CHECK_KMQUATERNION(L, idx) (*(kmQuaternion **) luaL_checkudata(L, idx, KAZMATH_CLS_KMQUATERNION))
#define KAZMATH_CHECK_KMMAT4(L, idx) (*(kmMat4 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMMAT4))
#define KAZMATH_CHECK_KMAABB2(L, idx) (*(kmAABB2 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMAABB2))
#define KAZMATH_CHECK_KMPLANE(L, idx) (*(kmPlane **) luaL_checkudata(L, idx, KAZMATH_CLS_KMPLANE))
#define KAZMATH_CHECK_KMVEC2(L, idx) (*(kmVec2 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMVEC2))
#define KAZMATH_CHECK_KMVEC3(L, idx) (*(kmVec3 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMVEC3))
#define KAZMATH_CHECK_KMVEC4(L, idx) (*(kmVec4 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMVEC4))
#define KAZMATH_CHECK_KMAABB3(L, idx) (*(kmAABB3 **) luaL_checkudata(L, idx, KAZMATH_CLS_KMAABB3))

#define KAZMATH_KMVEC2_FIELD_MAP(XX) XX(x, x) XX(y, y)
#define KAZMATH_KMVEC3_FIELD_MAP(XX) XX(x, x) XX(y, y) XX(z, z)
#define KAZMATH_KMVEC4_FIELD_MAP(XX) XX(x, x) XX(y, y) XX(z, z) XX(w, w)
#define KAZMATH_KMQUATERNION_FIELD_MAP(XX) XX(x, x) XX(y, y) XX(z, z) XX(w, w)
#define KAZMATH_KMPLANE_FIELD_MAP(XX) XX(a, a) XX(b, b) XX(c, c) XX(d, d)

#define KAZMATH_KMRAY2_FIELD_MAP(XX)           \
	XX(px, start.x) XX(py, start.y)        \
	XX(vx, dir.x) XX(vy, dir.y)

#define KAZMATH_KMRAY3_FIELD_MAP(XX)                           \
	XX(px, start.x) XX(py, start.y) XX(pz, start.z)        \
	XX(vx, dir.x) XX(vy, dir.y) XX(vz, dir.z)

#define VEC2_TO_ARRAY(L, vec) do {       \
	lua_newtable(L);                 \
	lua_pushnumber(L, vec->x);       \
	lua_rawseti(L, -2, 1);           \
	lua_pushnumber(L, vec->y);       \
	lua_rawseti(L, -2, 2);           \
} while(0)

#define VEC3_TO_ARRAY(L, vec) do {       \
	lua_newtable(L);                 \
	lua_pushnumber(L, vec->x);       \
	lua_rawseti(L, -2, 1);           \
	lua_pushnumber(L, vec->y);       \
	lua_rawseti(L, -2, 2);           \
	lua_pushnumber(L, vec->z);       \
	lua_rawseti(L, -2, 3);           \
} while(0)

#define VEC4_TO_ARRAY(L, vec) do {       \
	lua_newtable(L);                 \
	lua_pushnumber(L, vec->x);       \
	lua_rawseti(L, -2, 1);           \
	lua_pushnumber(L, vec->y);       \
	lua_rawseti(L, -2, 2);           \
	lua_pushnumber(L, vec->z);       \
	lua_rawseti(L, -2, 3);           \
	lua_pushnumber(L, vec->w);       \
	lua_rawseti(L, -2, 4);           \
} while(0)

#define MAT_TO_ARRAY(L, pmat) do {                                                \
	int idx;                                                                  \
	lua_newtable(L);                                                          \
	for (idx=0; idx < sizeof(pmat->mat)/sizeof(pmat->mat[0]); idx++) {        \
	        lua_pushnumber(L, pmat->mat[idx]);                                \
	        lua_rawseti(L, -2, idx+1);                                        \
	}                                                                         \
} while(0)

static int luac__is_array(lua_State *L, int idx)
{
	int isarray = 0;
	int len = 0;
	int top = lua_gettop(L);
	lua_pushvalue(L, idx);
	if (!lua_istable(L, -1))
		goto finished;

	len = lua_objlen(L, -1);
	if (len > 0) {
		lua_pushnumber(L, len);
		if (lua_next(L,-2) == 0) {
			isarray = 1;
		}
	}
finished:
	lua_settop(L, top);
	return isarray;
}

/**
 * kmQuaternion * kmQuaternionLookRotation(kmQuaternion * pOut, const kmVec3 * direction, const kmVec3 * up)
 */
static int lua__kmQuaternionLookRotation(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmVec3 * direction = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * up = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmQuaternionLookRotation(pOut, direction, up);
	return 0;
}

/**
 * kmRay3 * kmRay3Fill(kmRay3 * ray, kmScalar px, kmScalar py, kmScalar pz, kmScalar vx, kmScalar vy, kmScalar vz)
 */
static int lua__kmRay3Fill(lua_State *L)
{
	kmRay3 * ray = (kmRay3 *)KAZMATH_CHECK_KMRAY3(L, 1);
	kmScalar px = (kmScalar)luaL_checknumber(L, 2);
	kmScalar py = (kmScalar)luaL_checknumber(L, 3);
	kmScalar pz = (kmScalar)luaL_checknumber(L, 4);
	kmScalar vx = (kmScalar)luaL_checknumber(L, 5);
	kmScalar vy = (kmScalar)luaL_checknumber(L, 6);
	kmScalar vz = (kmScalar)luaL_checknumber(L, 7);
	kmRay3Fill(ray, px, py, pz, vx, vy, vz);
	return 0;
}

/**
 * kmVec2 * kmVec2Normalize(kmVec2 * pOut, const kmVec2 * pIn)
 */
static int lua__kmVec2Normalize(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pIn = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	kmVec2Normalize(pOut, pIn);
	return 0;
}

/**
 * kmPlane * kmPlaneFromPointAndNormal(kmPlane * pOut, const struct kmVec3 * pPoint, const struct kmVec3 * pNormal)
 */
static int lua__kmPlaneFromPointAndNormal(lua_State *L)
{
	kmPlane * pOut = (kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const struct kmVec3 * pPoint = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmVec3 * pNormal = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmPlaneFromPointAndNormal(pOut, pPoint, pNormal);
	return 0;
}

/**
 * kmVec4 * kmVec4Subtract(kmVec4 * pOut, const kmVec4 * pV1, const kmVec4 * pV2)
 */
static int lua__kmVec4Subtract(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pV1 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	const kmVec4 * pV2 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 3);
	kmVec4Subtract(pOut, pV1, pV2);
	return 0;
}

/**
 * kmScalar kmDegreesToRadians(kmScalar degrees)
 */
static int lua__kmDegreesToRadians(lua_State *L)
{
	kmScalar ret;
	kmScalar degrees = (kmScalar)luaL_checknumber(L, 1);
	ret = kmDegreesToRadians(degrees);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec2 * kmVec2Fill(kmVec2 * pOut, kmScalar x, kmScalar y)
 */
static int lua__kmVec2Fill(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	kmScalar x = (kmScalar)luaL_checknumber(L, 2);
	kmScalar y = (kmScalar)luaL_checknumber(L, 3);
	kmVec2Fill(pOut, x, y);
	return 0;
}

/**
 * kmVec2 * kmVec2MidPointBetween(kmVec2 * pOut, const kmVec2 * v1, const kmVec2 * v2)
 */
static int lua__kmVec2MidPointBetween(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * v1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * v2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmVec2MidPointBetween(pOut, v1, v2);
	return 0;
}

/**
 * kmVec2 * kmVec2TransformCoord(kmVec2 * pOut, const kmVec2 * pV, const struct kmMat3 * pM)
 */
static int lua__kmVec2TransformCoord(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const struct kmMat3 * pM = (const struct kmMat3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec2TransformCoord(pOut, pV, pM);
	return 0;
}

/**
 * kmScalar kmVec4Length(const kmVec4 * pIn)
 */
static int lua__kmVec4Length(lua_State *L)
{
	kmScalar ret;
	const kmVec4 * pIn = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	ret = kmVec4Length(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmQuaternion * kmQuaternionInverse(kmQuaternion * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionInverse(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionInverse(pOut, pIn);
	return 0;
}

/**
 * kmMat3 * kmMat3AssignMat3(kmMat3 * pOut, const kmMat3 * pIn)
 */
static int lua__kmMat3AssignMat3(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmMat3 * pIn = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	kmMat3AssignMat3(pOut, pIn);
	return 0;
}

/**
 * kmMat4 * kmMat4AssignMat3(kmMat4 * pOut, const struct kmMat3 * pIn)
 */
static int lua__kmMat4AssignMat3(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const struct kmMat3 * pIn = (const struct kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	kmMat4AssignMat3(pOut, pIn);
	return 0;
}

/**
 * kmScalar kmSQR(kmScalar s)
 */
static int lua__kmSQR(lua_State *L)
{
	kmScalar ret;
	kmScalar s = (kmScalar)luaL_checknumber(L, 1);
	ret = kmSQR(s);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * KM_POINT_CLASSIFICATION kmPlaneClassifyPoint(const kmPlane * pIn, const struct kmVec3 * pP)
 */
static int lua__kmPlaneClassifyPoint(lua_State *L)
{
	KM_POINT_CLASSIFICATION ret;
	const kmPlane * pIn = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const struct kmVec3 * pP = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	ret = kmPlaneClassifyPoint(pIn, pP);
	lua_pushinteger(L, ret);
	return 1;
}

/**
 * kmScalar kmAABB3DiameterZ(const kmAABB3 * aabb)
 */
static int lua__kmAABB3DiameterZ(lua_State *L)
{
	kmScalar ret;
	const kmAABB3 * aabb = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	ret = kmAABB3DiameterZ(aabb);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec3 * kmVec3Mul(kmVec3 * pOut, const kmVec3 * pV1, const kmVec3 * pV2)
 */
static int lua__kmVec3Mul(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * pV2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec3Mul(pOut, pV1, pV2);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionMultiply(kmQuaternion * pOut, const kmQuaternion * q1, const kmQuaternion * q2)
 */
static int lua__kmQuaternionMultiply(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * q1 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	const kmQuaternion * q2 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 3);
	kmQuaternionMultiply(pOut, q1, q2);
	return 0;
}

/**
 * kmVec4 * kmVec4Div(kmVec4 * pOut, const kmVec4 * pV1, const kmVec4 * pV2)
 */
static int lua__kmVec4Div(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pV1 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	const kmVec4 * pV2 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 3);
	kmVec4Div(pOut, pV1, pV2);
	return 0;
}

/**
 * kmMat4 * kmMat4RotationQuaternion(kmMat4 * pOut, const struct kmQuaternion * pQ)
 */
static int lua__kmMat4RotationQuaternion(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const struct kmQuaternion * pQ = (const struct kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmMat4RotationQuaternion(pOut, pQ);
	return 0;
}

/**
 * kmMat4 * kmMat4Translation(kmMat4 * pOut, const kmScalar x, const kmScalar y, const kmScalar z)
 */
static int lua__kmMat4Translation(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmScalar x = (const kmScalar)luaL_checknumber(L, 2);
	const kmScalar y = (const kmScalar)luaL_checknumber(L, 3);
	const kmScalar z = (const kmScalar)luaL_checknumber(L, 4);
	kmMat4Translation(pOut, x, y, z);
	return 0;
}

/**
 * struct kmVec3 * kmMat3ExtractUpVec3(const kmMat3 * self, struct kmVec3 * pOut)
 */
static int lua__kmMat3ExtractUpVec3(lua_State *L)
{
	const kmMat3 * self = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmMat3ExtractUpVec3(self, pOut);
	return 0;
}

/**
 * kmVec3 * kmAABB3Centre(const kmAABB3 * aabb, kmVec3 * pOut)
 */
static int lua__kmAABB3Centre(lua_State *L)
{
	const kmAABB3 * aabb = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmAABB3Centre(aabb, pOut);
	return 0;
}

/**
 * kmAABB2 * kmAABB2ExpandToContain(kmAABB2 * pOut, const kmAABB2 * pIn, const kmAABB2 * other)
 */
static int lua__kmAABB2ExpandToContain(lua_State *L)
{
	kmAABB2 * pOut = (kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmAABB2 * pIn = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 2);
	const kmAABB2 * other = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 3);
	kmAABB2ExpandToContain(pOut, pIn, other);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionNormalize(kmQuaternion * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionNormalize(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionNormalize(pOut, pIn);
	return 0;
}

/**
 * kmPlane * kmPlaneExtractFromMat4(kmPlane * pOut, const struct kmMat4 * pIn, kmInt row)
 */
static int lua__kmPlaneExtractFromMat4(lua_State *L)
{
	kmPlane * pOut = (kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const struct kmMat4 * pIn = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmInt row = (kmInt)luaL_checkinteger(L, 3);
	kmPlaneExtractFromMat4(pOut, pIn, row);
	return 0;
}

/**
 * kmBool kmSegment2WithSegmentIntersection(const kmRay2 * segmentA, const kmRay2 * segmentB, kmVec2 * intersection)
 */
static int lua__kmSegment2WithSegmentIntersection(lua_State *L)
{
	kmBool ret;
	const kmRay2 * segmentA = (const kmRay2 *)KAZMATH_CHECK_KMRAY2(L, 1);
	const kmRay2 * segmentB = (const kmRay2 *)KAZMATH_CHECK_KMRAY2(L, 2);
	kmVec2 * intersection = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	ret = kmSegment2WithSegmentIntersection(segmentA, segmentB, intersection);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmVec2 * kmVec2Transform(kmVec2 * pOut, const kmVec2 * pV1, const struct kmMat3 * pM)
 */
static int lua__kmVec2Transform(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const struct kmMat3 * pM = (const struct kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 3);
	kmVec2Transform(pOut, pV1, pM);
	return 0;
}

/**
 * kmVec2 * kmAABB2Centre(const kmAABB2 * aabb, kmVec2 * pOut)
 */
static int lua__kmAABB2Centre(lua_State *L)
{
	const kmAABB2 * aabb = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	kmAABB2Centre(aabb, pOut);
	return 0;
}

/**
 * kmMat3 * kmMat3FromRotationQuaternion(kmMat3 * pOut, const struct kmQuaternion * quaternion)
 */
static int lua__kmMat3FromRotationQuaternion(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const struct kmQuaternion * quaternion = (const struct kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmMat3FromRotationQuaternion(pOut, quaternion);
	return 0;
}

/**
 * kmVec2 * kmVec2Subtract(kmVec2 * pOut, const kmVec2 * pV1, const kmVec2 * pV2)
 */
static int lua__kmVec2Subtract(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * pV2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmVec2Subtract(pOut, pV1, pV2);
	return 0;
}

/**
 * kmAABB3 * kmAABB3Assign(kmAABB3 * pOut, const kmAABB3 * pIn)
 */
static int lua__kmAABB3Assign(lua_State *L)
{
	kmAABB3 * pOut = (kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	const kmAABB3 * pIn = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 2);
	kmAABB3Assign(pOut, pIn);
	return 0;
}

/**
 * kmMat4 * kmMat4Scaling(kmMat4 * pOut, const kmScalar x, const kmScalar y, const kmScalar z)
 */
static int lua__kmMat4Scaling(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmScalar x = (const kmScalar)luaL_checknumber(L, 2);
	const kmScalar y = (const kmScalar)luaL_checknumber(L, 3);
	const kmScalar z = (const kmScalar)luaL_checknumber(L, 4);
	kmMat4Scaling(pOut, x, y, z);
	return 0;
}

/**
 * struct kmMat3 * kmMat4ExtractRotationMat3(const kmMat4 * pIn, struct kmMat3 * pOut)
 */
static int lua__kmMat4ExtractRotationMat3(lua_State *L)
{
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	struct kmMat3 * pOut = (struct kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	kmMat4ExtractRotationMat3(pIn, pOut);
	return 0;
}

/**
 * kmVec3 * kmVec3InverseTransformNormal(kmVec3 * pOut, const kmVec3 * pVect, const struct kmMat4 * pM)
 */
static int lua__kmVec3InverseTransformNormal(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pVect = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmVec3InverseTransformNormal(pOut, pVect, pM);
	return 0;
}

/**
 * kmAABB2 * kmAABB2Initialize(kmAABB2 * pBox, const kmVec2 * centre, const kmScalar width, const kmScalar height, const kmScalar depth)
 */
static int lua__kmAABB2Initialize(lua_State *L)
{
	kmAABB2 * pBox = (kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmVec2 * centre = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmScalar width = (const kmScalar)luaL_checknumber(L, 3);
	const kmScalar height = (const kmScalar)luaL_checknumber(L, 4);
	const kmScalar depth = (const kmScalar)luaL_checknumber(L, 5);
	kmAABB2Initialize(pBox, centre, width, height, depth);
	return 0;
}

/**
 * kmBool kmAABB3IntersectsAABB(const kmAABB3 * box, const kmAABB3 * other)
 */
static int lua__kmAABB3IntersectsAABB(lua_State *L)
{
	kmBool ret;
	const kmAABB3 * box = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	const kmAABB3 * other = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 2);
	ret = kmAABB3IntersectsAABB(box, other);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmMat4 * kmMat4Fill(kmMat4 * pOut, const kmScalar * pMat)
 */
static int lua__kmMat4Fill(lua_State *L)
{
	const size_t rlen = sizeof(kmMat4)/sizeof(kmScalar);
	kmScalar pMat[sizeof(kmMat4)/sizeof(kmScalar)];
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);

	KAZMATH_CHECK_ARRAY_LEN(L, 2, rlen);
	KAZMATH_FILL_ARRAY(L, 2, pMat, kmScalar, lua_tonumber);

	kmMat4Fill(pOut, pMat);
	return 0;
}

/**
 * kmAABB3 * kmAABB3ExpandToContain(kmAABB3 * pOut, const kmAABB3 * pIn, const kmAABB3 * other)
 */
static int lua__kmAABB3ExpandToContain(lua_State *L)
{
	kmAABB3 * pOut = (kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	const kmAABB3 * pIn = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 2);
	const kmAABB3 * other = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 3);
	kmAABB3ExpandToContain(pOut, pIn, other);
	return 0;
}

/**
 * kmPlane * kmPlaneFromPoints(kmPlane * pOut, const struct kmVec3 * p1, const struct kmVec3 * p2, const struct kmVec3 * p3)
 */
static int lua__kmPlaneFromPoints(lua_State *L)
{
	kmPlane * pOut = (kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const struct kmVec3 * p1 = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmVec3 * p2 = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	const struct kmVec3 * p3 = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 4);
	kmPlaneFromPoints(pOut, p1, p2, p3);
	return 0;
}

/**
 * kmBool kmMat3IsIdentity(const kmMat3 * pIn)
 */
static int lua__kmMat3IsIdentity(lua_State *L)
{
	kmBool ret;
	const kmMat3 * pIn = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	ret = kmMat3IsIdentity(pIn);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmMat4 * kmMat4Identity(kmMat4 * pOut)
 */
static int lua__kmMat4Identity(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	kmMat4Identity(pOut);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionFill(kmQuaternion * pOut, kmScalar x, kmScalar y, kmScalar z, kmScalar w)
 */
static int lua__kmQuaternionFill(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	kmScalar x = (kmScalar)luaL_checknumber(L, 2);
	kmScalar y = (kmScalar)luaL_checknumber(L, 3);
	kmScalar z = (kmScalar)luaL_checknumber(L, 4);
	kmScalar w = (kmScalar)luaL_checknumber(L, 5);
	kmQuaternionFill(pOut, x, y, z, w);
	return 0;
}

/**
 * kmPlane * kmPlaneNormalize(kmPlane * pOut, const kmPlane * pP)
 */
static int lua__kmPlaneNormalize(lua_State *L)
{
	kmPlane * pOut = (kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const kmPlane * pP = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 2);
	kmPlaneNormalize(pOut, pP);
	return 0;
}

/**
 * kmVec4 * kmVec4Assign(kmVec4 * pOut, const kmVec4 * pIn)
 */
static int lua__kmVec4Assign(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pIn = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	kmVec4Assign(pOut, pIn);
	return 0;
}

/**
 * kmScalar kmQuaternionGetRoll(const kmQuaternion * q)
 */
static int lua__kmQuaternionGetRoll(lua_State *L)
{
	kmScalar ret;
	const kmQuaternion * q = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	ret = kmQuaternionGetRoll(q);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmAABB3 * kmAABB3Initialize(kmAABB3 * pBox, const kmVec3 * centre, const kmScalar width, const kmScalar height, const kmScalar depth)
 */
static int lua__kmAABB3Initialize(lua_State *L)
{
	kmAABB3 * pBox = (kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	const kmVec3 * centre = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmScalar width = (const kmScalar)luaL_checknumber(L, 3);
	const kmScalar height = (const kmScalar)luaL_checknumber(L, 4);
	const kmScalar depth = (const kmScalar)luaL_checknumber(L, 5);
	kmAABB3Initialize(pBox, centre, width, height, depth);
	return 0;
}

/**
 * kmBool kmMat3AreEqual(const kmMat3 * pMat1, const kmMat3 * pMat2)
 */
static int lua__kmMat3AreEqual(lua_State *L)
{
	kmBool ret;
	const kmMat3 * pMat1 = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmMat3 * pMat2 = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	ret = kmMat3AreEqual(pMat1, pMat2);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmVec4 * kmVec4Normalize(kmVec4 * pOut, const kmVec4 * pIn)
 */
static int lua__kmVec4Normalize(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pIn = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	kmVec4Normalize(pOut, pIn);
	return 0;
}

/**
 * kmMat3 * kmMat3FromRotationXInDegrees(kmMat3 * pOut, const kmScalar degrees)
 */
static int lua__kmMat3FromRotationXInDegrees(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmScalar degrees = (const kmScalar)luaL_checknumber(L, 2);
	kmMat3FromRotationXInDegrees(pOut, degrees);
	return 0;
}

/**
 * kmVec3 * kmVec3ProjectOnToPlane(kmVec3 * pOut, const kmVec3 * point, const struct kmPlane * plane)
 */
static int lua__kmVec3ProjectOnToPlane(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * point = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmPlane * plane = (const struct kmPlane *)KAZMATH_CHECK_KMPLANE(L, 3);
	kmVec3ProjectOnToPlane(pOut, point, plane);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionScale(kmQuaternion * pOut, const kmQuaternion * pIn, kmScalar s)
 */
static int lua__kmQuaternionScale(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmScalar s = (kmScalar)luaL_checknumber(L, 3);
	kmQuaternionScale(pOut, pIn, s);
	return 0;
}

/**
 * struct kmVec3 * kmQuaternionMultiplyVec3(struct kmVec3 * pOut, const kmQuaternion * q, const struct kmVec3 * v)
 */
static int lua__kmQuaternionMultiplyVec3(lua_State *L)
{
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmQuaternion * q = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	const struct kmVec3 * v = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmQuaternionMultiplyVec3(pOut, q, v);
	return 0;
}

/**
 * kmScalar kmVec2Dot(const kmVec2 * pV1, const kmVec2 * pV2)
 */
static int lua__kmVec2Dot(lua_State *L)
{
	kmScalar ret;
	const kmVec2 * pV1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	ret = kmVec2Dot(pV1, pV2);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * void kmRay2FillWithEndpoints(kmRay2 * ray, const kmVec2 * start, const kmVec2 * end)
 */
static int lua__kmRay2FillWithEndpoints(lua_State *L)
{
	kmRay2 * ray = (kmRay2 *)KAZMATH_CHECK_KMRAY2(L, 1);
	const kmVec2 * start = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * end = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmRay2FillWithEndpoints(ray, start, end);
	return 0;
}

/**
 * kmMat3 * kmMat3Inverse(kmMat3 * pOut, const kmMat3 * pM)
 */
static int lua__kmMat3Inverse(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmMat3 * pM = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	kmMat3Inverse(pOut, pM);
	return 0;
}

/**
 * kmMat3 * kmMat3Transpose(kmMat3 * pOut, const kmMat3 * pIn)
 */
static int lua__kmMat3Transpose(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmMat3 * pIn = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	kmMat3Transpose(pOut, pIn);
	return 0;
}

/**
 * int kmQuaternionIsIdentity(const kmQuaternion * pIn)
 */
static int lua__kmQuaternionIsIdentity(lua_State *L)
{
	int ret;
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	ret = kmQuaternionIsIdentity(pIn);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmQuaternion * kmQuaternionSlerp(kmQuaternion * pOut, const kmQuaternion * q1, const kmQuaternion * q2, kmScalar t)
 */
static int lua__kmQuaternionSlerp(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * q1 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	const kmQuaternion * q2 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 3);
	kmScalar t = (kmScalar)luaL_checknumber(L, 4);
	kmQuaternionSlerp(pOut, q1, q2, t);
	return 0;
}

/**
 * kmVec3 * kmVec3Lerp(kmVec3 * pOut, const kmVec3 * pV1, const kmVec3 * pV2, kmScalar t)
 */
static int lua__kmVec3Lerp(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * pV2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmScalar t = (kmScalar)luaL_checknumber(L, 4);
	kmVec3Lerp(pOut, pV1, pV2, t);
	return 0;
}

/**
 * kmAABB3 * kmAABB3Scale(kmAABB3 * pOut, const kmAABB3 * pIn, kmScalar s)
 */
static int lua__kmAABB3Scale(lua_State *L)
{
	kmAABB3 * pOut = (kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	const kmAABB3 * pIn = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 2);
	kmScalar s = (kmScalar)luaL_checknumber(L, 3);
	kmAABB3Scale(pOut, pIn, s);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionRotationBetweenVec3(kmQuaternion * pOut, const struct kmVec3 * vec1, const struct kmVec3 * vec2, const struct kmVec3 * fallback)
 */
static int lua__kmQuaternionRotationBetweenVec3(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const struct kmVec3 * vec1 = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmVec3 * vec2 = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	const struct kmVec3 * fallback = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 4);
	kmQuaternionRotationBetweenVec3(pOut, vec1, vec2, fallback);
	return 0;
}

/**
 * kmScalar kmVec3LengthSq(const kmVec3 * pIn)
 */
static int lua__kmVec3LengthSq(lua_State *L)
{
	kmScalar ret;
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	ret = kmVec3LengthSq(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmMat4 * kmMat4LookAt(kmMat4 * pOut, const struct kmVec3 * pEye, const struct kmVec3 * pCenter, const struct kmVec3 * pUp)
 */
static int lua__kmMat4LookAt(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const struct kmVec3 * pEye = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmVec3 * pCenter = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	const struct kmVec3 * pUp = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 4);
	kmMat4LookAt(pOut, pEye, pCenter, pUp);
	return 0;
}

/**
 * kmScalar kmClamp(kmScalar x, kmScalar min, kmScalar max)
 */
static int lua__kmClamp(lua_State *L)
{
	kmScalar ret;
	kmScalar x = (kmScalar)luaL_checknumber(L, 1);
	kmScalar min = (kmScalar)luaL_checknumber(L, 2);
	kmScalar max = (kmScalar)luaL_checknumber(L, 3);
	ret = kmClamp(x, min, max);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmMat3 * kmMat3Adjugate(kmMat3 * pOut, const kmMat3 * pIn)
 */
static int lua__kmMat3Adjugate(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmMat3 * pIn = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	kmMat3Adjugate(pOut, pIn);
	return 0;
}

/**
 * int kmVec2AreEqual(const kmVec2 * p1, const kmVec2 * p2)
 */
static int lua__kmVec2AreEqual(lua_State *L)
{
	int ret;
	const kmVec2 * p1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * p2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	ret = kmVec2AreEqual(p1, p2);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * struct kmVec3 * kmMat4GetForwardVec3LH(struct kmVec3 * pOut, const kmMat4 * pIn)
 */
static int lua__kmMat4GetForwardVec3LH(lua_State *L)
{
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmMat4GetForwardVec3LH(pOut, pIn);
	return 0;
}

/**
 * kmEnum kmAABB2ContainsAABB(const kmAABB2 * container, const kmAABB2 * to_check)
 */
static int lua__kmAABB2ContainsAABB(lua_State *L)
{
	kmEnum ret;
	const kmAABB2 * container = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmAABB2 * to_check = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 2);
	ret = kmAABB2ContainsAABB(container, to_check);
	lua_pushinteger(L, (lua_Integer)ret);
	return 1;
}

/**
 * kmBool kmRay3IntersectTriangle(const kmRay3 * ray, const kmVec3 * v0, const kmVec3 * v1, const kmVec3 * v2, kmVec3 * intersection, kmVec3 * normal, kmScalar * distance)
 */
static int lua__kmRay3IntersectTriangle(lua_State *L)
{
	kmBool ret;
	kmScalar distance = 0.0;
	const kmRay3 * ray = (const kmRay3 *)KAZMATH_CHECK_KMRAY3(L, 1);
	const kmVec3 * v0 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * v1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	const kmVec3 * v2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 4);
	kmVec3 * intersection = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 5);
	kmVec3 * normal = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 6);
	ret = kmRay3IntersectTriangle(ray, v0, v1, v2, intersection, normal, &distance);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, distance);
	return 2;
}

/**
 * kmMat3 * kmMat3FromScaling(kmMat3 * pOut, const kmScalar x, const kmScalar y)
 */
static int lua__kmMat3FromScaling(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmScalar x = (const kmScalar)luaL_checknumber(L, 2);
	const kmScalar y = (const kmScalar)luaL_checknumber(L, 3);
	kmMat3FromScaling(pOut, x, y);
	return 0;
}

/**
 * kmScalar kmQuaternionGetPitch(const kmQuaternion * q)
 */
static int lua__kmQuaternionGetPitch(lua_State *L)
{
	kmScalar ret;
	const kmQuaternion * q = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	ret = kmQuaternionGetPitch(q);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec4 * kmVec4Mul(kmVec4 * pOut, const kmVec4 * pV1, const kmVec4 * pV2)
 */
static int lua__kmVec4Mul(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pV1 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	const kmVec4 * pV2 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 3);
	kmVec4Mul(pOut, pV1, pV2);
	return 0;
}

/**
 * void kmVec3OrthoNormalize(kmVec3 * normal, kmVec3 * tangent)
 */
static int lua__kmVec3OrthoNormalize(lua_State *L)
{
	kmVec3 * normal = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	kmVec3 * tangent = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmVec3OrthoNormalize(normal, tangent);
	return 0;
}

/**
 * int kmMat4AreEqual(const kmMat4 * pM1, const kmMat4 * pM2)
 */
static int lua__kmMat4AreEqual(lua_State *L)
{
	int ret;
	const kmMat4 * pM1 = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmMat4 * pM2 = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	ret = kmMat4AreEqual(pM1, pM2);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmScalar kmPlaneDot(const kmPlane * pP, const struct kmVec4 * pV)
 */
static int lua__kmPlaneDot(lua_State *L)
{
	kmScalar ret;
	const kmPlane * pP = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const struct kmVec4 * pV = (const struct kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	ret = kmPlaneDot(pP, pV);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmQuaternion * kmQuaternionIdentity(kmQuaternion * pOut)
 */
static int lua__kmQuaternionIdentity(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	kmQuaternionIdentity(pOut);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionRotationMatrix(kmQuaternion * pOut, const struct kmMat3 * pIn)
 */
static int lua__kmQuaternionRotationMatrix(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const struct kmMat3 * pIn = (const struct kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	kmQuaternionRotationMatrix(pOut, pIn);
	return 0;
}

/**
 * struct kmVec3 * kmMat4GetRightVec3(struct kmVec3 * pOut, const kmMat4 * pIn)
 */
static int lua__kmMat4GetRightVec3(lua_State *L)
{
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmMat4GetRightVec3(pOut, pIn);
	return 0;
}

/**
 * kmScalar kmQuaternionLengthSq(const kmQuaternion * pIn)
 */
static int lua__kmQuaternionLengthSq(lua_State *L)
{
	kmScalar ret;
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	ret = kmQuaternionLengthSq(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmQuaternion * kmQuaternionAssign(kmQuaternion * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionAssign(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionAssign(pOut, pIn);
	return 0;
}

/**
 * kmScalar kmVec2LengthSq(const kmVec2 * pIn)
 */
static int lua__kmVec2LengthSq(lua_State *L)
{
	kmScalar ret;
	const kmVec2 * pIn = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	ret = kmVec2LengthSq(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmScalar kmMat3Determinant(const kmMat3 * pIn)
 */
static int lua__kmMat3Determinant(lua_State *L)
{
	kmScalar ret;
	const kmMat3 * pIn = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	ret = kmMat3Determinant(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmMat3 * kmMat3Fill(kmMat3 * pOut, const kmScalar * pMat)
 */
static int lua__kmMat3Fill(lua_State *L)
{
	const size_t rlen = sizeof(kmMat3)/sizeof(kmScalar);
	kmScalar pMat[sizeof(kmMat3)/sizeof(kmScalar)];
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	KAZMATH_CHECK_ARRAY_LEN(L, 2, rlen);
	KAZMATH_FILL_ARRAY(L, 2, pMat, kmScalar, lua_tonumber);
	kmMat3Fill(pOut, pMat);
	return 0;
}

/**
 * struct kmVec3 * kmMat4RotationToAxisAngle(struct kmVec3 * pAxis, kmScalar * radians, const kmMat4 * pIn)
 */
static int lua__kmMat4RotationToAxisAngle(lua_State *L)
{
	kmScalar radians = 0.0;
	struct kmVec3 * pAxis = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	/* NOTE By Roc */
	/* args removed */
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmMat4RotationToAxisAngle(pAxis, &radians, pIn);
	lua_pushnumber(L, radians);
	return 1;
}

/**
 * kmBool kmRay2IntersectCircle(const kmRay2 * ray, const kmVec2 centre, const kmScalar radius, kmVec2 * intersection)
 */
static int lua__kmRay2IntersectCircle(lua_State *L)
{
	kmBool ret;
	const kmRay2 * ray = (const kmRay2 *)KAZMATH_CHECK_KMRAY2(L, 1);
	(void) ret;
	(void) ray;
	return luaL_error(L, "Not implemented");
	/*
	const kmVec2 centre = (const kmVec2)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmScalar radius = (const kmScalar)luaL_checknumber(L, 3);
	kmVec2 * intersection = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 4);
	ret = kmRay2IntersectCircle(ray, centre, radius, intersection);
	lua_pushinteger(L, (lua_Integer)ret);
	return 0;
	*/
}

/**
 * kmRay3 * kmRay3FromPointAndDirection(kmRay3 * ray, const kmVec3 * point, const kmVec3 * direction)
 */
static int lua__kmRay3FromPointAndDirection(lua_State *L)
{
	kmRay3 * ray = (kmRay3 *)KAZMATH_CHECK_KMRAY3(L, 1);
	const kmVec3 * point = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * direction = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmRay3FromPointAndDirection(ray, point, direction);
	return 0;
}

/**
 * kmVec3 * kmVec3Transform(kmVec3 * pOut, const kmVec3 * pV1, const struct kmMat4 * pM)
 */
static int lua__kmVec3Transform(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmVec3Transform(pOut, pV1, pM);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionAdd(kmQuaternion * pOut, const kmQuaternion * pQ1, const kmQuaternion * pQ2)
 */
static int lua__kmQuaternionAdd(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * pQ1 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	const kmQuaternion * pQ2 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 3);
	kmQuaternionAdd(pOut, pQ1, pQ2);
	return 0;
}

/**
 * kmScalar kmVec4Dot(const kmVec4 * pV1, const kmVec4 * pV2)
 */
static int lua__kmVec4Dot(lua_State *L)
{
	kmScalar ret;
	const kmVec4 * pV1 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pV2 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	ret = kmVec4Dot(pV1, pV2);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec3 * kmVec3GetHorizontalAngle(kmVec3 * pOut, const kmVec3 * pIn)
 */
static int lua__kmVec3GetHorizontalAngle(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmVec3GetHorizontalAngle(pOut, pIn);
	return 0;
}

/**
 * kmScalar kmRadiansToDegrees(kmScalar radians)
 */
static int lua__kmRadiansToDegrees(lua_State *L)
{
	kmScalar ret;
	kmScalar radians = (kmScalar)luaL_checknumber(L, 1);
	ret = kmRadiansToDegrees(radians);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec3 * kmQuaternionGetUpVec3(kmVec3 * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionGetUpVec3(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionGetUpVec3(pOut, pIn);
	return 0;
}

/**
 * kmVec2 * kmVec2RotateBy(kmVec2 * pOut, const kmVec2 * pIn, const kmScalar degrees, const kmVec2 * center)
 */
static int lua__kmVec2RotateBy(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pIn = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmScalar degrees = (const kmScalar)luaL_checknumber(L, 3);
	const kmVec2 * center = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 4);
	kmVec2RotateBy(pOut, pIn, degrees, center);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionExp(kmQuaternion * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionExp(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionExp(pOut, pIn);
	return 0;
}

/**
 * kmVec3 * kmVec3Zero(kmVec3 * pOut)
 */
static int lua__kmVec3Zero(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	kmVec3Zero(pOut);
	return 0;
}

/**
 * kmMat3 * kmMat3MultiplyScalar(kmMat3 * pOut, const kmMat3 * lhs, const kmScalar rhs)
 */
static int lua__kmMat3MultiplyScalar(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmMat3 * lhs = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	const kmScalar rhs = (const kmScalar)luaL_checknumber(L, 3);
	kmMat3MultiplyScalar(pOut, lhs, rhs);
	return 0;
}

/**
 * kmScalar kmQuaternionDot(const kmQuaternion * q1, const kmQuaternion * q2)
 */
static int lua__kmQuaternionDot(lua_State *L)
{
	kmScalar ret;
	const kmQuaternion * q1 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * q2 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	ret = kmQuaternionDot(q1, q2);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec4 * kmVec4Scale(kmVec4 * pOut, const kmVec4 * pIn, const kmScalar s)
 */
static int lua__kmVec4Scale(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pIn = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	const kmScalar s = (const kmScalar)luaL_checknumber(L, 3);
	kmVec4Scale(pOut, pIn, s);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionLn(kmQuaternion * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionLn(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionLn(pOut, pIn);
	return 0;
}

/**
 * void kmQuaternionToAxisAngle(const kmQuaternion * pIn, struct kmVec3 * pVector, kmScalar * pAngle)
 */
static int lua__kmQuaternionToAxisAngle(lua_State *L)
{
	kmScalar angle = 0.0;
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	struct kmVec3 * pVector = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmQuaternionToAxisAngle(pIn, pVector, &angle);
	lua_pushnumber(L, angle);
	return 1;
}

/**
 * void kmRay2Fill(kmRay2 * ray, kmScalar px, kmScalar py, kmScalar vx, kmScalar vy)
 */
static int lua__kmRay2Fill(lua_State *L)
{
	kmRay2 * ray = (kmRay2 *)KAZMATH_CHECK_KMRAY2(L, 1);
	kmScalar px = (kmScalar)luaL_checknumber(L, 2);
	kmScalar py = (kmScalar)luaL_checknumber(L, 3);
	kmScalar vx = (kmScalar)luaL_checknumber(L, 4);
	kmScalar vy = (kmScalar)luaL_checknumber(L, 5);
	kmRay2Fill(ray, px, py, vx, vy);
	return 0;
}

/**
 * struct kmVec3 * kmMat4ExtractTranslationVec3(const kmMat4 * pIn, struct kmVec3 * pOut)
 */
static int lua__kmMat4ExtractTranslationVec3(lua_State *L)
{
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmMat4ExtractTranslationVec3(pIn, pOut);
	return 1;
}

/**
 * kmVec3 * kmVec3Normalize(kmVec3 * pOut, const kmVec3 * pIn)
 */
static int lua__kmVec3Normalize(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmVec3Normalize(pOut, pIn);
	return 0;
}

/**
 * kmVec3 * kmVec3RotationToDirection(kmVec3 * pOut, const kmVec3 * pIn, const kmVec3 * forwards)
 */
static int lua__kmVec3RotationToDirection(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * forwards = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec3RotationToDirection(pOut, pIn, forwards);
	return 0;
}

/**
 * kmVec4 * kmVec4Transform(kmVec4 * pOut, const kmVec4 * pV, const struct kmMat4 * pM)
 */
static int lua__kmVec4Transform(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pV = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmVec4Transform(pOut, pV, pM);
	return 0;
}

/**
 * kmVec3 * kmVec3Reflect(kmVec3 * pOut, const kmVec3 * pIn, const kmVec3 * normal)
 */
static int lua__kmVec3Reflect(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * normal = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec3Reflect(pOut, pIn, normal);
	return 0;
}

/**
 * kmAABB2 * kmAABB2ScaleWithPivot(kmAABB2 * pOut, const kmAABB2 * pIn, const kmVec2 * pivot, kmScalar s)
 */
static int lua__kmAABB2ScaleWithPivot(lua_State *L)
{
	kmAABB2 * pOut = (kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmAABB2 * pIn = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 2);
	const kmVec2 * pivot = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmScalar s = (kmScalar)luaL_checknumber(L, 4);
	kmAABB2ScaleWithPivot(pOut, pIn, pivot, s);
	return 0;
}

/**
 * int kmAABB3ContainsPoint(const kmAABB3 * pBox, const kmVec3 * pPoint)
 */
static int lua__kmAABB3ContainsPoint(lua_State *L)
{
	int ret;
	const kmAABB3 * pBox = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	const kmVec3 * pPoint = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	ret = kmAABB3ContainsPoint(pBox, pPoint);
	lua_pushinteger(L, (lua_Integer)ret);
	return 1;
}

/**
 * kmScalar kmAABB2DiameterY(const kmAABB2 * aabb)
 */
static int lua__kmAABB2DiameterY(lua_State *L)
{
	kmScalar ret;
	const kmAABB2 * aabb = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	ret = kmAABB2DiameterY(aabb);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmMat3 * kmMat3FromRotationYInDegrees(kmMat3 * pOut, const kmScalar degrees)
 */
static int lua__kmMat3FromRotationYInDegrees(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmScalar degrees = (const kmScalar)luaL_checknumber(L, 2);
	kmMat3FromRotationYInDegrees(pOut, degrees);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionSubtract(kmQuaternion * pOut, const kmQuaternion * pQ1, const kmQuaternion * pQ2)
 */
static int lua__kmQuaternionSubtract(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * pQ1 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	const kmQuaternion * pQ2 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 3);
	kmQuaternionSubtract(pOut, pQ1, pQ2);
	return 0;
}

/**
 * kmScalar kmAABB2DiameterX(const kmAABB2 * aabb)
 */
static int lua__kmAABB2DiameterX(lua_State *L)
{
	kmScalar ret;
	const kmAABB2 * aabb = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	ret = kmAABB2DiameterX(aabb);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec3 * kmVec3Subtract(kmVec3 * pOut, const kmVec3 * pV1, const kmVec3 * pV2)
 */
static int lua__kmVec3Subtract(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * pV2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec3Subtract(pOut, pV1, pV2);
	return 0;
}

/**
 * kmBool kmRay2IntersectBox(const kmRay2 * ray, const kmVec2 * p1, const kmVec2 * p2, const kmVec2 * p3, const kmVec2 * p4, kmVec2 * intersection, kmVec2 * normal_out)
 */
static int lua__kmRay2IntersectBox(lua_State *L)
{
	kmBool ret;
	const kmRay2 * ray = (const kmRay2 *)KAZMATH_CHECK_KMRAY2(L, 1);
	const kmVec2 * p1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * p2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	const kmVec2 * p3 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 4);
	const kmVec2 * p4 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 5);
	kmVec2 * intersection = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 6);
	kmVec2 * normal_out = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 7);
	ret = kmRay2IntersectBox(ray, p1, p2, p3, p4, intersection, normal_out);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmVec3 * kmQuaternionGetForwardVec3RH(kmVec3 * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionGetForwardVec3RH(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionGetForwardVec3RH(pOut, pIn);
	return 0;
}

/**
 * kmBool kmRay2IntersectTriangle(const kmRay2 * ray, const kmVec2 * p1, const kmVec2 * p2, const kmVec2 * p3, kmVec2 * intersection, kmVec2 * normal_out, kmScalar * distance)
 */
static int lua__kmRay2IntersectTriangle(lua_State *L)
{
	kmBool ret;
	kmScalar distance = 0.0;
	const kmRay2 * ray = (const kmRay2 *)KAZMATH_CHECK_KMRAY2(L, 1);
	const kmVec2 * p1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * p2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	const kmVec2 * p3 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 4);
	kmVec2 * intersection = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 5);
	kmVec2 * normal_out = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 6);
	ret = kmRay2IntersectTriangle(ray, p1, p2, p3, intersection, normal_out, &distance);
	lua_pushboolean(L, ret);
	lua_pushinteger(L, distance);
	return 2;
}

/**
 * kmAABB2 * kmAABB2Sanitize(kmAABB2 * pOut, const kmAABB2 * pIn)
 */
static int lua__kmAABB2Sanitize(lua_State *L)
{
	kmAABB2 * pOut = (kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmAABB2 * pIn = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 2);
	kmAABB2Sanitize(pOut, pIn);
	return 0;
}

/**
 * void kmVec2Swap(kmVec2 * pA, kmVec2 * pB)
 */
static int lua__kmVec2Swap(lua_State *L)
{
	kmVec2 * pA = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	kmVec2 * pB = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	kmVec2Swap(pA, pB);
	return 0;
}

/**
 * kmVec2 * kmVec2Div(kmVec2 * pOut, const kmVec2 * pV1, const kmVec2 * pV2)
 */
static int lua__kmVec2Div(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * pV2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmVec2Div(pOut, pV1, pV2);
	return 0;
}

/**
 * int kmVec4AreEqual(const kmVec4 * p1, const kmVec4 * p2)
 */
static int lua__kmVec4AreEqual(lua_State *L)
{
	int ret;
	const kmVec4 * p1 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * p2 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	ret = kmVec4AreEqual(p1, p2);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmVec4 * kmVec4Add(kmVec4 * pOut, const kmVec4 * pV1, const kmVec4 * pV2)
 */
static int lua__kmVec4Add(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pV1 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	const kmVec4 * pV2 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 3);
	kmVec4Add(pOut, pV1, pV2);
	return 0;
}

/**
 * kmMat3 * kmMat3FromRotationZInDegrees(kmMat3 * pOut, const kmScalar degrees)
 */
static int lua__kmMat3FromRotationZInDegrees(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmScalar degrees = (const kmScalar)luaL_checknumber(L, 2);
	kmMat3FromRotationZInDegrees(pOut, degrees);
	return 0;
}

/**
 * kmScalar kmPlaneDotNormal(const kmPlane * pP, const struct kmVec3 * pV)
 */
static int lua__kmPlaneDotNormal(lua_State *L)
{
	kmScalar ret;
	const kmPlane * pP = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const struct kmVec3 * pV = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	ret = kmPlaneDotNormal(pP, pV);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec3 * kmVec3Fill(kmVec3 * pOut, kmScalar x, kmScalar y, kmScalar z)
 */
static int lua__kmVec3Fill(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	kmScalar x = (kmScalar)luaL_checknumber(L, 2);
	kmScalar y = (kmScalar)luaL_checknumber(L, 3);
	kmScalar z = (kmScalar)luaL_checknumber(L, 4);
	kmVec3Fill(pOut, x, y, z);
	return 0;
}

/**
 * kmVec3 * kmVec3TransformCoord(kmVec3 * pOut, const kmVec3 * pV, const struct kmMat4 * pM)
 */
static int lua__kmVec3TransformCoord(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmVec3TransformCoord(pOut, pV, pM);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionRotationPitchYawRoll(kmQuaternion * pOut, kmScalar pitch, kmScalar yaw, kmScalar roll)
 */
static int lua__kmQuaternionRotationPitchYawRoll(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	kmScalar pitch = (kmScalar)luaL_checknumber(L, 2);
	kmScalar yaw = (kmScalar)luaL_checknumber(L, 3);
	kmScalar roll = (kmScalar)luaL_checknumber(L, 4);
	kmQuaternionRotationPitchYawRoll(pOut, pitch, yaw, roll);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionBetweenVec3(kmQuaternion * pOut, const kmVec3 * v1, const kmVec3 * v2)
 */
static int lua__kmQuaternionBetweenVec3(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmVec3 * v1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * v2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmQuaternionBetweenVec3(pOut, v1, v2);
	return 0;
}

/**
 * kmMat3 * kmMat3Identity(kmMat3 * pOut)
 */
static int lua__kmMat3Identity(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	kmMat3Identity(pOut);
	return 0;
}

/**
 * kmVec2 * kmVec2Reflect(kmVec2 * pOut, const kmVec2 * pIn, const kmVec2 * normal)
 */
static int lua__kmVec2Reflect(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pIn = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * normal = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmVec2Reflect(pOut, pIn, normal);
	return 0;
}

/**
 * kmScalar kmVec3Length(const kmVec3 * pIn)
 */
static int lua__kmVec3Length(lua_State *L)
{
	kmScalar ret;
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	ret = kmVec3Length(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * int kmQuaternionAreEqual(const kmQuaternion * p1, const kmQuaternion * p2)
 */
static int lua__kmQuaternionAreEqual(lua_State *L)
{
	int ret;
	const kmQuaternion * p1 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmQuaternion * p2 = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	ret = kmQuaternionAreEqual(p1, p2);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmScalar kmVec4LengthSq(const kmVec4 * pIn)
 */
static int lua__kmVec4LengthSq(lua_State *L)
{
	kmScalar ret;
	const kmVec4 * pIn = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	ret = kmVec4LengthSq(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmScalar kmVec2DegreesBetween(const kmVec2 * v1, const kmVec2 * v2)
 */
static int lua__kmVec2DegreesBetween(lua_State *L)
{
	kmScalar ret;
	const kmVec2 * v1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * v2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	ret = kmVec2DegreesBetween(v1, v2);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmScalar kmVec3Dot(const kmVec3 * pV1, const kmVec3 * pV2)
 */
static int lua__kmVec3Dot(lua_State *L)
{
	kmScalar ret;
	const kmVec3 * pV1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	ret = kmVec3Dot(pV1, pV2);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmBool kmRay2IntersectLineSegment(const kmRay2 * ray, const kmVec2 * p1, const kmVec2 * p2, kmVec2 * intersection)
 */
static int lua__kmRay2IntersectLineSegment(lua_State *L)
{
	kmBool ret;
	const kmRay2 * ray = (const kmRay2 *)KAZMATH_CHECK_KMRAY2(L, 1);
	const kmVec2 * p1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * p2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmVec2 * intersection = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 4);
	ret = kmRay2IntersectLineSegment(ray, p1, p2, intersection);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmAABB2 * kmAABB2Assign(kmAABB2 * pOut, const kmAABB2 * pIn)
 */
static int lua__kmAABB2Assign(lua_State *L)
{
	kmAABB2 * pOut = (kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmAABB2 * pIn = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 2);
	kmAABB2Assign(pOut, pIn);
	return 0;
}

/**
 * kmVec4 * kmVec4Lerp(kmVec4 * pOut, const kmVec4 * pV1, const kmVec4 * pV2, kmScalar t)
 */
static int lua__kmVec4Lerp(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pV1 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	const kmVec4 * pV2 = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 3);
	kmScalar t = (kmScalar)luaL_checknumber(L, 4);
	kmVec4Lerp(pOut, pV1, pV2, t);
	return 0;
}

/**
 * kmScalar kmAABB3DiameterX(const kmAABB3 * aabb)
 */
static int lua__kmAABB3DiameterX(lua_State *L)
{
	kmScalar ret;
	const kmAABB3 * aabb = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	ret = kmAABB3DiameterX(aabb);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmScalar kmAABB3DiameterY(const kmAABB3 * aabb)
 */
static int lua__kmAABB3DiameterY(lua_State *L)
{
	kmScalar ret;
	const kmAABB3 * aabb = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	ret = kmAABB3DiameterY(aabb);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmPlane * kmPlaneFill(kmPlane * plane, kmScalar a, kmScalar b, kmScalar c, kmScalar d)
 */
static int lua__kmPlaneFill(lua_State *L)
{
	kmPlane * plane = (kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	kmScalar a = (kmScalar)luaL_checknumber(L, 2);
	kmScalar b = (kmScalar)luaL_checknumber(L, 3);
	kmScalar c = (kmScalar)luaL_checknumber(L, 4);
	kmScalar d = (kmScalar)luaL_checknumber(L, 5);
	kmPlaneFill(plane, a, b, c, d);
	return 0;
}

/**
 * int kmAABB2ContainsPoint(const kmAABB2 * pBox, const kmVec2 * pPoint)
 */
static int lua__kmAABB2ContainsPoint(lua_State *L)
{
	int ret;
	const kmAABB2 * pBox = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmVec2 * pPoint = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	ret = kmAABB2ContainsPoint(pBox, pPoint);
	lua_pushinteger(L, (lua_Integer)ret);
	return 1;
}

/**
 * kmVec2 * kmVec2Add(kmVec2 * pOut, const kmVec2 * pV1, const kmVec2 * pV2)
 */
static int lua__kmVec2Add(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * pV2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmVec2Add(pOut, pV1, pV2);
	return 0;
}

/**
 * kmMat4 * kmMat4OrthographicProjection(kmMat4 * pOut, kmScalar left, kmScalar right, kmScalar bottom, kmScalar top, kmScalar nearVal, kmScalar farVal)
 */
static int lua__kmMat4OrthographicProjection(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	kmScalar left = (kmScalar)luaL_checknumber(L, 2);
	kmScalar right = (kmScalar)luaL_checknumber(L, 3);
	kmScalar bottom = (kmScalar)luaL_checknumber(L, 4);
	kmScalar top = (kmScalar)luaL_checknumber(L, 5);
	kmScalar nearVal = (kmScalar)luaL_checknumber(L, 6);
	kmScalar farVal = (kmScalar)luaL_checknumber(L, 7);
	kmMat4OrthographicProjection(pOut, left, right, bottom, top, nearVal, farVal);
	return 0;
}

/**
 * kmBool kmRay3IntersectPlane(kmVec3 * pOut, const kmRay3 * ray, const struct kmPlane * plane)
 */
static int lua__kmRay3IntersectPlane(lua_State *L)
{
	kmBool ret;
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmRay3 * ray = (const kmRay3 *)KAZMATH_CHECK_KMRAY3(L, 2);
	const struct kmPlane * plane = (const struct kmPlane *)KAZMATH_CHECK_KMPLANE(L, 3);
	ret = kmRay3IntersectPlane(pOut, ray, plane);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmMat4 * kmMat4Assign(kmMat4 * pOut, const kmMat4 * pIn)
 */
static int lua__kmMat4Assign(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmMat4Assign(pOut, pIn);
	return 0;
}

/**
 * struct kmVec3 * kmPlaneGetIntersection(struct kmVec3 * pOut, const kmPlane * p1, const kmPlane * p2, const kmPlane * p3)
 */
static int lua__kmPlaneGetIntersection(lua_State *L)
{
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmPlane * p1 = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 2);
	const kmPlane * p2 = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 3);
	const kmPlane * p3 = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 4);
	kmPlaneGetIntersection(pOut, p1, p2, p3);
	return 0;
}

/**
 * kmVec3 * kmVec3TransformNormal(kmVec3 * pOut, const kmVec3 * pV, const struct kmMat4 * pM)
 */
static int lua__kmVec3TransformNormal(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmVec3TransformNormal(pOut, pV, pM);
	return 0;
}

/**
 * struct kmVec3 * kmMat4GetForwardVec3RH(struct kmVec3 * pOut, const kmMat4 * pIn)
 */
static int lua__kmMat4GetForwardVec3RH(lua_State *L)
{
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmMat4GetForwardVec3RH(pOut, pIn);
	return 0;
}

/**
 * kmBool kmVec3AreEqual(const kmVec3 * p1, const kmVec3 * p2)
 */
static int lua__kmVec3AreEqual(lua_State *L)
{
	kmBool ret;
	const kmVec3 * p1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * p2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	ret = kmVec3AreEqual(p1, p2);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmVec4 * kmVec4MultiplyMat4(kmVec4 * pOut, const kmVec4 * pV, const struct kmMat4 * pM)
 */
static int lua__kmVec4MultiplyMat4(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	const kmVec4 * pV = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmVec4MultiplyMat4(pOut, pV, pM);
	return 0;
}

/**
 * kmVec3 * kmVec3Cross(kmVec3 * pOut, const kmVec3 * pV1, const kmVec3 * pV2)
 */
static int lua__kmVec3Cross(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * pV2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec3Cross(pOut, pV1, pV2);
	return 0;
}

/**
 * kmScalar kmVec2DistanceBetween(const kmVec2 * v1, const kmVec2 * v2)
 */
static int lua__kmVec2DistanceBetween(lua_State *L)
{
	kmScalar ret;
	const kmVec2 * v1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * v2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	ret = kmVec2DistanceBetween(v1, v2);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec3 * kmVec3Div(kmVec3 * pOut, const kmVec3 * pV1, const kmVec3 * pV2)
 */
static int lua__kmVec3Div(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * pV2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec3Div(pOut, pV1, pV2);
	return 0;
}

/**
 * kmBool kmLine2WithLineIntersection(const kmVec2 * ptA, const kmVec2 * vecA, const kmVec2 * ptB, const kmVec2 * vecB, kmScalar * outTA, kmScalar * outTB, kmVec2 * outIntersection)
 */
static int lua__kmLine2WithLineIntersection(lua_State *L)
{
	kmBool ret;
	kmScalar outTA = 0.0;
	kmScalar outTB = 0.0;
	const kmVec2 * ptA = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * vecA = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * ptB = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	const kmVec2 * vecB = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 4);
	/* NOTE By Roc */
	/* args removed */
	kmVec2 * outIntersection = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 5);
	ret = kmLine2WithLineIntersection(ptA, vecA, ptB, vecB, &outTA, &outTB, outIntersection);
	lua_pushboolean(L, ret);
	lua_pushnumber(L, outTA);
	lua_pushnumber(L, outTB);
	return 3;
}

/**
 * struct kmVec3 * kmMat4GetUpVec3(struct kmVec3 * pOut, const kmMat4 * pIn)
 */
static int lua__kmMat4GetUpVec3(lua_State *L)
{
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmMat4GetUpVec3(pOut, pIn);
	return 0;
}

/**
 * kmMat4 * kmMat4Transpose(kmMat4 * pOut, const kmMat4 * pIn)
 */
static int lua__kmMat4Transpose(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmMat4Transpose(pOut, pIn);
	return 0;
}


/**
 * kmScalar kmLerp(kmScalar x, kmScalar y, kmScalar factor)
 */
static int lua__kmLerp(lua_State *L)
{
	kmScalar ret;
	kmScalar x = (kmScalar)luaL_checknumber(L, 1);
	kmScalar y = (kmScalar)luaL_checknumber(L, 2);
	kmScalar factor = (kmScalar)luaL_checknumber(L, 3);
	ret = kmLerp(x, y, factor);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmMat4 * kmMat4RotationTranslation(kmMat4 * pOut, const struct kmMat3 * rotation, const struct kmVec3 * translation)
 */
static int lua__kmMat4RotationTranslation(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const struct kmMat3 * rotation = (const struct kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	const struct kmVec3 * translation = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmMat4RotationTranslation(pOut, rotation, translation);
	return 0;
}

/**
 * kmEnum kmAABB3ContainsAABB(const kmAABB3 * container, const kmAABB3 * to_check)
 */
static int lua__kmAABB3ContainsAABB(lua_State *L)
{
	kmEnum ret;
	const kmAABB3 * container = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	const kmAABB3 * to_check = (const kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 2);
	ret = kmAABB3ContainsAABB(container, to_check);
	lua_pushinteger(L, (lua_Integer)ret);
	return 1;
}

/**
 * struct kmPlane * kmMat4ExtractPlane(struct kmPlane * pOut, const kmMat4 * pIn, const kmEnum plane)
 */
static int lua__kmMat4ExtractPlane(lua_State *L)
{
	struct kmPlane * pOut = (struct kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	const kmEnum plane = (const kmEnum)luaL_checkinteger(L, 3);
	kmMat4ExtractPlane(pOut, pIn, plane);
	return 0;
}

/**
 * kmVec2 * kmVec2Lerp(kmVec2 * pOut, const kmVec2 * pV1, const kmVec2 * pV2, kmScalar t)
 */
static int lua__kmVec2Lerp(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * pV2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmScalar t = (kmScalar)luaL_checknumber(L, 4);
	kmVec2Lerp(pOut, pV1, pV2, t);
	return 0;
}

/**
 * kmMat3 * kmMat3FromRotationZ(kmMat3 * pOut, const kmScalar radians)
 */
static int lua__kmMat3FromRotationZ(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmScalar radians = (const kmScalar)luaL_checknumber(L, 2);
	kmMat3FromRotationZ(pOut, radians);
	return 0;
}

/**
 * kmMat3 * kmMat3FromRotationX(kmMat3 * pOut, const kmScalar radians)
 */
static int lua__kmMat3FromRotationX(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmScalar radians = (const kmScalar)luaL_checknumber(L, 2);
	kmMat3FromRotationX(pOut, radians);
	return 0;
}

/**
 * kmMat3 * kmMat3FromRotationY(kmMat3 * pOut, const kmScalar radians)
 */
static int lua__kmMat3FromRotationY(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmScalar radians = (const kmScalar)luaL_checknumber(L, 2);
	kmMat3FromRotationY(pOut, radians);
	return 0;
}

/**
 * kmVec3 * kmVec3ProjectOnToVec3(const kmVec3 * pIn, const kmVec3 * other, kmVec3 * projection)
 */
static int lua__kmVec3ProjectOnToVec3(lua_State *L)
{
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * other = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmVec3 * projection = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec3ProjectOnToVec3(pIn, other, projection);
	return 0;
}

/**
 * kmVec4 * kmVec4TransformArray(kmVec4 * pOut, unsigned int outStride, const kmVec4 * pV, unsigned int vStride, const struct kmMat4 * pM, unsigned int count)
 */
static int lua__kmVec4TransformArray(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	unsigned int outStride = (unsigned int)luaL_checkinteger(L, 2);
	const kmVec4 * pV = (const kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 3);
	unsigned int vStride = (unsigned int)luaL_checkinteger(L, 4);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 5);
	unsigned int count = (unsigned int)luaL_checkinteger(L, 6);
	kmVec4TransformArray(pOut, outStride, pV, vStride, pM, count);
	return 0;
}

/**
 * kmMat3 * kmMat3FromTranslation(kmMat3 * pOut, const kmScalar x, const kmScalar y)
 */
static int lua__kmMat3FromTranslation(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmScalar x = (const kmScalar)luaL_checknumber(L, 2);
	const kmScalar y = (const kmScalar)luaL_checknumber(L, 3);
	kmMat3FromTranslation(pOut, x, y);
	return 0;
}

/**
 * kmAABB2 * kmAABB2Translate(kmAABB2 * pOut, const kmAABB2 * pIn, const kmVec2 * translation)
 */
static int lua__kmAABB2Translate(lua_State *L)
{
	kmAABB2 * pOut = (kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmAABB2 * pIn = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 2);
	const kmVec2 * translation = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmAABB2Translate(pOut, pIn, translation);
	return 0;
}

/**
 * kmScalar kmVec2Cross(const kmVec2 * pV1, const kmVec2 * pV2)
 */
static int lua__kmVec2Cross(lua_State *L)
{
	kmScalar ret;
	const kmVec2 * pV1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	ret = kmVec2Cross(pV1, pV2);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmMat4 * kmMat4PerspectiveProjection(kmMat4 * pOut, kmScalar fovY, kmScalar aspect, kmScalar zNear, kmScalar zFar)
 */
static int lua__kmMat4PerspectiveProjection(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	kmScalar fovY = (kmScalar)luaL_checknumber(L, 2);
	kmScalar aspect = (kmScalar)luaL_checknumber(L, 3);
	kmScalar zNear = (kmScalar)luaL_checknumber(L, 4);
	kmScalar zFar = (kmScalar)luaL_checknumber(L, 5);
	kmMat4PerspectiveProjection(pOut, fovY, aspect, zNear, zFar);
	return 0;
}

/**
 * struct kmVec3 * kmPlaneIntersectLine(struct kmVec3 * pOut, const kmPlane * pP, const struct kmVec3 * pV1, const struct kmVec3 * pV2)
 */
static int lua__kmPlaneIntersectLine(lua_State *L)
{
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmPlane * pP = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 2);
	const struct kmVec3 * pV1 = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	const struct kmVec3 * pV2 = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 4);
	kmPlaneIntersectLine(pOut, pP, pV1, pV2);
	return 0;
}

/**
 * kmScalar kmVec2Length(const kmVec2 * pIn)
 */
static int lua__kmVec2Length(lua_State *L)
{
	kmScalar ret;
	const kmVec2 * pIn = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	ret = kmVec2Length(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * struct kmVec3 * kmMat3ExtractRightVec3(const kmMat3 * self, struct kmVec3 * pOut)
 */
static int lua__kmMat3ExtractRightVec3(lua_State *L)
{
	const kmMat3 * self = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmMat3ExtractRightVec3(self, pOut);
	return 0;
}

/**
 * void kmVec3Swap(kmVec3 * a, kmVec3 * b)
 */
static int lua__kmVec3Swap(lua_State *L)
{
	kmVec3 * a = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	kmVec3 * b = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmVec3Swap(a, b);
	return 0;
}

/**
 * kmVec3 * kmVec3InverseTransform(kmVec3 * pOut, const kmVec3 * pV, const struct kmMat4 * pM)
 */
static int lua__kmVec3InverseTransform(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmVec3InverseTransform(pOut, pV, pM);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionExtractRotationAroundAxis(const kmQuaternion * pIn, const kmVec3 * axis, kmQuaternion * pOut)
 */
static int lua__kmQuaternionExtractRotationAroundAxis(lua_State *L)
{
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const kmVec3 * axis = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 3);
	kmQuaternionExtractRotationAroundAxis(pIn, axis, pOut);
	return 0;
}

/**
 * kmScalar kmQuaternionLength(const kmQuaternion * pIn)
 */
static int lua__kmQuaternionLength(lua_State *L)
{
	kmScalar ret;
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	ret = kmQuaternionLength(pIn);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmScalar kmQuaternionGetYaw(const kmQuaternion * q)
 */
static int lua__kmQuaternionGetYaw(lua_State *L)
{
	kmScalar ret;
	const kmQuaternion * q = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	ret = kmQuaternionGetYaw(q);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmVec3 * kmQuaternionGetForwardVec3LH(kmVec3 * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionGetForwardVec3LH(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionGetForwardVec3LH(pOut, pIn);
	return 0;
}

/**
 * kmVec3 * kmVec3Scale(kmVec3 * pOut, const kmVec3 * pIn, const kmScalar s)
 */
static int lua__kmVec3Scale(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmScalar s = (const kmScalar)luaL_checknumber(L, 3);
	kmVec3Scale(pOut, pIn, s);
	return 0;
}

/**
 * kmMat4 * kmMat4Inverse(kmMat4 * pOut, const kmMat4 * pM)
 */
static int lua__kmMat4Inverse(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmMat4 * pM = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	kmMat4Inverse(pOut, pM);
	return 0;
}

/**
 * kmVec4 * kmVec4Fill(kmVec4 * pOut, kmScalar x, kmScalar y, kmScalar z, kmScalar w)
 */
static int lua__kmVec4Fill(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	kmScalar x = (kmScalar)luaL_checknumber(L, 2);
	kmScalar y = (kmScalar)luaL_checknumber(L, 3);
	kmScalar z = (kmScalar)luaL_checknumber(L, 4);
	kmScalar w = (kmScalar)luaL_checknumber(L, 5);
	kmVec4Fill(pOut, x, y, z, w);
	return 0;
}

/**
 * kmVec3 * kmVec3Add(kmVec3 * pOut, const kmVec3 * pV1, const kmVec3 * pV2)
 */
static int lua__kmVec3Add(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * pV2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	kmVec3Add(pOut, pV1, pV2);
	return 0;
}

/**
 * kmBool kmAlmostEqual(kmScalar lhs, kmScalar rhs)
 */
static int lua__kmAlmostEqual(lua_State *L)
{
	kmBool ret;
	kmScalar lhs = (kmScalar)luaL_checknumber(L, 1);
	kmScalar rhs = (kmScalar)luaL_checknumber(L, 2);
	ret = kmAlmostEqual(lhs, rhs);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * void kmMat3ExtractRotationAxisAngle(const kmMat3 * self, struct kmVec3 * axis, kmScalar * radians)
 */
static int lua__kmMat3ExtractRotationAxisAngle(lua_State *L)
{
	kmScalar radians = 0.0;
	const kmMat3 * self = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	struct kmVec3 * axis = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmMat3ExtractRotationAxisAngle(self, axis, &radians);
	lua_pushnumber(L, radians);
	return 1;
}

/**
 * kmAABB2 * kmAABB2Scale(kmAABB2 * pOut, const kmAABB2 * pIn, kmScalar s)
 */
static int lua__kmAABB2Scale(lua_State *L)
{
	kmAABB2 * pOut = (kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 1);
	const kmAABB2 * pIn = (const kmAABB2 *)KAZMATH_CHECK_KMAABB2(L, 2);
	kmScalar s = (kmScalar)luaL_checknumber(L, 3);
	kmAABB2Scale(pOut, pIn, s);
	return 0;
}

/**
 * kmPlane * kmPlaneScale(kmPlane * pOut, const kmPlane * pP, kmScalar s)
 */
static int lua__kmPlaneScale(lua_State *L)
{
	kmPlane * pOut = (kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const kmPlane * pP = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 2);
	kmScalar s = (kmScalar)luaL_checknumber(L, 3);
	kmPlaneScale(pOut, pP, s);
	return 0;
}

/**
 * kmVec3 * kmQuaternionGetRightVec3(kmVec3 * pOut, const kmQuaternion * pIn)
 */
static int lua__kmQuaternionGetRightVec3(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmQuaternion * pIn = (const kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 2);
	kmQuaternionGetRightVec3(pOut, pIn);
	return 0;
}

/**
 * kmScalar kmMax(kmScalar lhs, kmScalar rhs)
 */
static int lua__kmMax(lua_State *L)
{
	kmScalar ret;
	kmScalar lhs = (kmScalar)luaL_checknumber(L, 1);
	kmScalar rhs = (kmScalar)luaL_checknumber(L, 2);
	ret = kmMax(lhs, rhs);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * void kmVec4Swap(kmVec4 * pA, kmVec4 * pB)
 */
static int lua__kmVec4Swap(lua_State *L)
{
	kmVec4 * pA = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	kmVec4 * pB = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 2);
	kmVec4Swap(pA, pB);
	return 0;
}

/**
 * struct kmVec3 * kmMat3ExtractForwardVec3(const kmMat3 * self, struct kmVec3 * pOut)
 */
static int lua__kmMat3ExtractForwardVec3(lua_State *L)
{
	const kmMat3 * self = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	struct kmVec3 * pOut = (struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmMat3ExtractForwardVec3(self, pOut);
	return 0;
}

/**
 * kmMat4 * kmMat4Multiply(kmMat4 * pOut, const kmMat4 * pM1, const kmMat4 * pM2)
 */
static int lua__kmMat4Multiply(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmMat4 * pM1 = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 2);
	const kmMat4 * pM2 = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmMat4Multiply(pOut, pM1, pM2);
	return 0;
}

/**
 * kmMat3 * kmMat3MultiplyMat3(kmMat3 * pOut, const kmMat3 * lhs, const kmMat3 * rhs)
 */
static int lua__kmMat3MultiplyMat3(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const kmMat3 * lhs = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 2);
	const kmMat3 * rhs = (const kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 3);
	kmMat3MultiplyMat3(pOut, lhs, rhs);
	return 0;
}

/**
 * kmMat3 * kmMat3FromRotationAxisAngle(kmMat3 * pOut, const struct kmVec3 * axis, const kmScalar radians)
 */
static int lua__kmMat3FromRotationAxisAngle(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const struct kmVec3 * axis = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmScalar radians = (const kmScalar)luaL_checknumber(L, 3);
	kmMat3FromRotationAxisAngle(pOut, axis, radians);
	return 0;
}

/**
 * kmVec2 * kmVec2Scale(kmVec2 * pOut, const kmVec2 * pIn, const kmScalar s)
 */
static int lua__kmVec2Scale(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pIn = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmScalar s = (const kmScalar)luaL_checknumber(L, 3);
	kmVec2Scale(pOut, pIn, s);
	return 0;
}

/**
 * kmMat3 * kmMat3FromRotationAxisAngleInDegrees(kmMat3 * pOut, const struct kmVec3 * axis, const kmScalar degrees)
 */
static int lua__kmMat3FromRotationAxisAngleInDegrees(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const struct kmVec3 * axis = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmScalar degrees = (const kmScalar)luaL_checknumber(L, 3);
	kmMat3FromRotationAxisAngleInDegrees(pOut, axis, degrees);
	return 0;
}

/**
 * kmBool kmAABB3IntersectsTriangle(kmAABB3 * box, const kmVec3 * p1, const kmVec3 * p2, const kmVec3 * p3)
 */
static int lua__kmAABB3IntersectsTriangle(lua_State *L)
{
	kmBool ret;
	kmAABB3 * box = (kmAABB3 *)KAZMATH_CHECK_KMAABB3(L, 1);
	const kmVec3 * p1 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmVec3 * p2 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	const kmVec3 * p3 = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 4);
	ret = kmAABB3IntersectsTriangle(box, p1, p2, p3);
	lua_pushinteger(L, (lua_Integer)ret);
	return 1;
}

/**
 * kmMat4 * kmMat4RotationYawPitchRoll(kmMat4 * pOut, const kmScalar pitch, const kmScalar yaw, const kmScalar roll)
 */
static int lua__kmMat4RotationYawPitchRoll(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmScalar pitch = (const kmScalar)luaL_checknumber(L, 2);
	const kmScalar yaw = (const kmScalar)luaL_checknumber(L, 3);
	const kmScalar roll = (const kmScalar)luaL_checknumber(L, 4);
	kmMat4RotationYawPitchRoll(pOut, pitch, yaw, roll);
	return 0;
}

/**
 * int kmMat4IsIdentity(const kmMat4 * pIn)
 */
static int lua__kmMat4IsIdentity(lua_State *L)
{
	int ret;
	const kmMat4 * pIn = (const kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	ret = kmMat4IsIdentity(pIn);
	lua_pushboolean(L, ret);
	return 1;
}

/**
 * kmMat3 * kmMat3FromRotationLookAt(kmMat3 * pOut, const struct kmVec3 * pEye, const struct kmVec3 * pCentre, const struct kmVec3 * pUp)
 */
static int lua__kmMat3FromRotationLookAt(lua_State *L)
{
	kmMat3 * pOut = (kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 1);
	const struct kmVec3 * pEye = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmVec3 * pCentre = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 3);
	const struct kmVec3 * pUp = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 4);
	kmMat3FromRotationLookAt(pOut, pEye, pCentre, pUp);
	return 0;
}

/**
 * kmVec2 * kmVec2Assign(kmVec2 * pOut, const kmVec2 * pIn)
 */
static int lua__kmVec2Assign(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pIn = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	kmVec2Assign(pOut, pIn);
	return 0;
}

/**
 * kmScalar kmPlaneDotCoord(const kmPlane * pP, const struct kmVec3 * pV)
 */
static int lua__kmPlaneDotCoord(lua_State *L)
{
	kmScalar ret;
	const kmPlane * pP = (const kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const struct kmVec3 * pV = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	ret = kmPlaneDotCoord(pP, pV);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmScalar kmMin(kmScalar lhs, kmScalar rhs)
 */
static int lua__kmMin(lua_State *L)
{
	kmScalar ret;
	kmScalar lhs = (kmScalar)luaL_checknumber(L, 1);
	kmScalar rhs = (kmScalar)luaL_checknumber(L, 2);
	ret = kmMin(lhs, rhs);
	lua_pushnumber(L, (lua_Number)ret);
	return 1;
}

/**
 * kmMat4 * kmMat4RotationZ(kmMat4 * pOut, const kmScalar radians)
 */
static int lua__kmMat4RotationZ(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmScalar radians = (const kmScalar)luaL_checknumber(L, 2);
	kmMat4RotationZ(pOut, radians);
	return 0;
}

/**
 * kmMat4 * kmMat4RotationY(kmMat4 * pOut, const kmScalar radians)
 */
static int lua__kmMat4RotationY(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmScalar radians = (const kmScalar)luaL_checknumber(L, 2);
	kmMat4RotationY(pOut, radians);
	return 0;
}

/**
 * kmMat4 * kmMat4RotationX(kmMat4 * pOut, const kmScalar radians)
 */
static int lua__kmMat4RotationX(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const kmScalar radians = (const kmScalar)luaL_checknumber(L, 2);
	kmMat4RotationX(pOut, radians);
	return 0;
}

/**
 * kmMat4 * kmMat4RotationAxisAngle(kmMat4 * pOut, const struct kmVec3 * axis, kmScalar radians)
 */
static int lua__kmMat4RotationAxisAngle(lua_State *L)
{
	kmMat4 * pOut = (kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 1);
	const struct kmVec3 * axis = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmScalar radians = (kmScalar)luaL_checknumber(L, 3);
	kmMat4RotationAxisAngle(pOut, axis, radians);
	return 0;
}

/**
 * kmVec3 * kmVec3Assign(kmVec3 * pOut, const kmVec3 * pIn)
 */
static int lua__kmVec3Assign(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pIn = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmVec3Assign(pOut, pIn);
	return 0;
}

/**
 * kmPlane * kmPlaneFromNormalAndDistance(kmPlane * plane, const struct kmVec3 * normal, const kmScalar dist)
 */
static int lua__kmPlaneFromNormalAndDistance(lua_State *L)
{
	kmPlane * plane = (kmPlane *)KAZMATH_CHECK_KMPLANE(L, 1);
	const struct kmVec3 * normal = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const kmScalar dist = (const kmScalar)luaL_checknumber(L, 3);
	kmPlaneFromNormalAndDistance(plane, normal, dist);
	return 0;
}

/**
 * kmVec3 * kmVec3MultiplyMat3(kmVec3 * pOut, const kmVec3 * pV, const struct kmMat3 * pM)
 */
static int lua__kmVec3MultiplyMat3(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmMat3 * pM = (const struct kmMat3 *)KAZMATH_CHECK_KMMAT3(L, 3);
	kmVec3MultiplyMat3(pOut, pV, pM);
	return 0;
}

/**
 * kmVec2 * kmVec2Mul(kmVec2 * pOut, const kmVec2 * pV1, const kmVec2 * pV2)
 */
static int lua__kmVec2Mul(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	const kmVec2 * pV1 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 2);
	const kmVec2 * pV2 = (const kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 3);
	kmVec2Mul(pOut, pV1, pV2);
	return 0;
}

/**
 * kmVec3 * kmVec3MultiplyMat4(kmVec3 * pOut, const kmVec3 * pV, const struct kmMat4 * pM)
 */
static int lua__kmVec3MultiplyMat4(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	const kmVec3 * pV = (const kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	const struct kmMat4 * pM = (const struct kmMat4 *)KAZMATH_CHECK_KMMAT4(L, 3);
	kmVec3MultiplyMat4(pOut, pV, pM);
	return 0;
}

/**
 * kmQuaternion * kmQuaternionRotationAxisAngle(kmQuaternion * pOut, const struct kmVec3 * pV, kmScalar angle)
 */
static int lua__kmQuaternionRotationAxisAngle(lua_State *L)
{
	kmQuaternion * pOut = (kmQuaternion *)KAZMATH_CHECK_KMQUATERNION(L, 1);
	const struct kmVec3 * pV = (const struct kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 2);
	kmScalar angle = (kmScalar)luaL_checknumber(L, 3);
	kmQuaternionRotationAxisAngle(pOut, pV, angle);
	return 0;
}



static int lua__kmMat3New(lua_State *L)
{
	kmMat3 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmMat3, p, KAZMATH_CLS_KMMAT3);
	return 1;
}

static int lua__kmMat3NewWithArray(lua_State *L)
{
	const size_t rlen = sizeof(kmMat3)/sizeof(kmScalar);
	kmScalar pMat[sizeof(kmMat3)/sizeof(kmScalar)];
	kmMat3 *p;
	KAZMATH_CHECK_ARRAY_LEN(L, 1, rlen);
	KAZMATH_FILL_ARRAY(L, 1, pMat, kmScalar, lua_tonumber);
	p = calloc(1, sizeof(*p));
	kmMat3Fill(p, pMat);
	KAZMATH_LUA_BIND_META(L, kmMat3, p, KAZMATH_CLS_KMMAT3);
	return 1;
}

static int lua__kmMat3_gc(lua_State *L)
{
	kmMat3 *p = KAZMATH_CHECK_KMMAT3(L, 1);
	free(p);
	return 0;
}

static int lua__kmMat3ToArray(lua_State *L)
{
	kmMat3 *p = KAZMATH_CHECK_KMMAT3(L, 1);
	MAT_TO_ARRAY(L, p);
	return 1;
}

static int lua__kmRay2New(lua_State *L)
{
	kmRay2 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmRay2, p, KAZMATH_CLS_KMRAY2);
	return 1;
}

static int lua__kmRay2_gc(lua_State *L)
{
	kmRay2 *p = KAZMATH_CHECK_KMRAY2(L, 1);
	free(p);
	return 0;
}

static int lua__kmRay2_newindex(lua_State *L)
{
	kmRay2 *ptr = KAZMATH_CHECK_KMRAY2(L, 1);
	KAZMATH_LUA_NEWINDEX_FILED(L, KAZMATH_KMRAY2_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmRay2_index(lua_State *L)
{
	kmRay2 *ptr = KAZMATH_CHECK_KMRAY2(L, 1);
	KAZMATH_LUA_INDEX_METHOD(L, KAZMATH_CLS_KMRAY2);
	KAZMATH_LUA_INDEX_FIELD(L, KAZMATH_KMRAY2_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmRay3New(lua_State *L)
{
	kmRay3 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmRay3, p, KAZMATH_CLS_KMRAY3);
	return 1;
}


static int lua__kmRay3_gc(lua_State *L)
{
	kmRay3 *p = KAZMATH_CHECK_KMRAY3(L, 1);
	free(p);
	return 0;
}

static int lua__kmRay3_newindex(lua_State *L)
{
	kmRay3 *ptr = KAZMATH_CHECK_KMRAY3(L, 1);
	KAZMATH_LUA_NEWINDEX_FILED(L, KAZMATH_KMRAY3_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmRay3_index(lua_State *L)
{
	kmRay3 *ptr = KAZMATH_CHECK_KMRAY3(L, 1);
	KAZMATH_LUA_INDEX_METHOD(L, KAZMATH_CLS_KMRAY3);
	KAZMATH_LUA_INDEX_FIELD(L, KAZMATH_KMRAY3_FIELD_MAP, ptr);
	return 0;
}


static int lua__kmQuaternionNew(lua_State *L)
{
	kmQuaternion *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmQuaternion, p, KAZMATH_CLS_KMQUATERNION);
	return 1;
}


static int lua__kmQuaternion_gc(lua_State *L)
{
	kmQuaternion *p = KAZMATH_CHECK_KMQUATERNION(L, 1);
	free(p);
	return 0;
}

static int lua__kmQuaternion_newindex(lua_State *L)
{
	kmQuaternion *ptr = KAZMATH_CHECK_KMQUATERNION(L, 1);
	KAZMATH_LUA_NEWINDEX_FILED(L, KAZMATH_KMQUATERNION_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmQuaternion_index(lua_State *L)
{
	kmQuaternion *ptr = KAZMATH_CHECK_KMQUATERNION(L, 1);
	KAZMATH_LUA_INDEX_METHOD(L, KAZMATH_CLS_KMQUATERNION);
	KAZMATH_LUA_INDEX_FIELD(L, KAZMATH_KMQUATERNION_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmMat4New(lua_State *L)
{
	kmMat4 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmMat4, p, KAZMATH_CLS_KMMAT4);
	return 1;
}

static int lua__kmMat4NewWithArray(lua_State *L)
{
	const size_t rlen = sizeof(kmMat4)/sizeof(kmScalar);
	kmScalar pMat[sizeof(kmMat4)/sizeof(kmScalar)];
	kmMat4 *p;
	KAZMATH_CHECK_ARRAY_LEN(L, 1, rlen);
	KAZMATH_FILL_ARRAY(L, 1, pMat, kmScalar, lua_tonumber);
	p = calloc(1, sizeof(*p));
	kmMat4Fill(p, pMat);
	KAZMATH_LUA_BIND_META(L, kmMat4, p, KAZMATH_CLS_KMMAT4);
	return 1;
}

static int lua__kmMat4_gc(lua_State *L)
{
	kmMat4 *p = KAZMATH_CHECK_KMMAT4(L, 1);
	free(p);
	return 0;
}

static int lua__kmMat4ToArray(lua_State *L)
{
	kmMat4 *p = KAZMATH_CHECK_KMMAT4(L, 1);
	MAT_TO_ARRAY(L, p);
	return 1;
}

static int lua__kmAABB2New(lua_State *L)
{
	kmAABB2 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmAABB2, p, KAZMATH_CLS_KMAABB2);
	return 1;
}

static int lua__kmAABB2_gc(lua_State *L)
{
	kmAABB2 *p = KAZMATH_CHECK_KMAABB2(L, 1);
	free(p);
	return 0;
}

static int lua__kmPlaneNew(lua_State *L)
{
	kmPlane *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmPlane, p, KAZMATH_CLS_KMPLANE);
	return 1;
}


static int lua__kmPlane_gc(lua_State *L)
{
	kmPlane *p = KAZMATH_CHECK_KMPLANE(L, 1);
	free(p);
	return 0;
}

static int lua__kmPlane_newindex(lua_State *L)
{
	kmPlane *ptr = KAZMATH_CHECK_KMPLANE(L, 1);
	KAZMATH_LUA_NEWINDEX_FILED(L, KAZMATH_KMPLANE_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmPlane_index(lua_State *L)
{
	kmPlane *ptr = KAZMATH_CHECK_KMPLANE(L, 1);
	KAZMATH_LUA_INDEX_METHOD(L, KAZMATH_CLS_KMPLANE);
	KAZMATH_LUA_INDEX_FIELD(L, KAZMATH_KMPLANE_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmVec2New(lua_State *L)
{
	kmVec2 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmVec2, p, KAZMATH_CLS_KMVEC2);
	return 1;
}

static int lua__kmVec2NewWithTbl(lua_State *L)
{
	kmVec2 *p;
	kmScalar x;
	kmScalar y;
	luaL_checktype(L, 1, LUA_TTABLE);
	do {
		lua_getfield(L, 1, "x");
		x = luaL_checknumber(L, -1);
		lua_getfield(L, 1, "y");
		y = luaL_checknumber(L, -1);
	} while (0);
	p = calloc(1, sizeof(*p));
	kmVec2Fill(p, x, y);
	KAZMATH_LUA_BIND_META(L, kmVec2, p, KAZMATH_CLS_KMVEC2);
	return 1;
}

static int lua__kmVec2_gc(lua_State *L)
{
	kmVec2 *p = KAZMATH_CHECK_KMVEC2(L, 1);
	free(p);
	return 0;
}

static int lua__kmVec2_newindex(lua_State *L)
{
	kmVec2 *ptr = KAZMATH_CHECK_KMVEC2(L, 1);
	KAZMATH_LUA_NEWINDEX_FILED(L, KAZMATH_KMVEC2_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmVec2_index(lua_State *L)
{
	kmVec2 * ptr = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	KAZMATH_LUA_INDEX_METHOD(L, KAZMATH_CLS_KMVEC2);
	KAZMATH_LUA_INDEX_FIELD(L, KAZMATH_KMVEC2_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmVec2ToArray(lua_State *L)
{
	kmVec2 * pOut = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	VEC2_TO_ARRAY(L, pOut);
	return 1;
}

static int lua__kmVec2ToTbl(lua_State *L)
{
	kmVec2 * pvec = (kmVec2 *)KAZMATH_CHECK_KMVEC2(L, 1);
	lua_newtable(L);
	lua_pushnumber(L, pvec->x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, pvec->y);
	lua_setfield(L, -2, "y");
	return 1;
}

static int lua__kmVec3New(lua_State *L)
{
	kmVec3 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmVec3, p, KAZMATH_CLS_KMVEC3);
	return 1;
}

static int lua__kmVec3NewWithTbl(lua_State *L)
{
	kmVec3 *p;
	kmScalar x;
	kmScalar y;
	kmScalar z;
	luaL_checktype(L, 1, LUA_TTABLE);
	do {
		lua_getfield(L, 1, "x");
		x = luaL_checknumber(L, -1);
		lua_getfield(L, 1, "y");
		y = luaL_checknumber(L, -1);
		lua_getfield(L, 1, "z");
		z = luaL_checknumber(L, -1);
	} while (0);
	p = calloc(1, sizeof(*p));
	kmVec3Fill(p, x, y, z);
	KAZMATH_LUA_BIND_META(L, kmVec3, p, KAZMATH_CLS_KMVEC3);
	return 1;
}

static int lua__kmVec3_gc(lua_State *L)
{
	kmVec3 *p = KAZMATH_CHECK_KMVEC3(L, 1);
	free(p);
	return 0;
}

static int lua__kmVec3_newindex(lua_State *L)
{
	kmVec3 *ptr = KAZMATH_CHECK_KMVEC3(L, 1);
	KAZMATH_LUA_NEWINDEX_FILED(L, KAZMATH_KMVEC3_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmVec3_index(lua_State *L)
{
	kmVec3 * ptr = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	KAZMATH_LUA_INDEX_METHOD(L, KAZMATH_CLS_KMVEC3);
	KAZMATH_LUA_INDEX_FIELD(L, KAZMATH_KMVEC3_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmVec3ToArray(lua_State *L)
{
	kmVec3 * pOut = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	VEC3_TO_ARRAY(L, pOut);
	return 1;
}

static int lua__kmVec3ToTbl(lua_State *L)
{
	kmVec3 * pvec = (kmVec3 *)KAZMATH_CHECK_KMVEC3(L, 1);
	lua_newtable(L);
	lua_pushnumber(L, pvec->x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, pvec->y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, pvec->z);
	lua_setfield(L, -2, "z");
	return 1;
}

static int lua__kmVec4New(lua_State *L)
{
	kmVec4 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmVec4, p, KAZMATH_CLS_KMVEC4);
	return 1;
}

static int lua__kmVec4NewWithTbl(lua_State *L)
{
	kmVec4 *p;
	kmScalar x;
	kmScalar y;
	kmScalar z;
	kmScalar w;
	luaL_checktype(L, 1, LUA_TTABLE);
	do {
		lua_getfield(L, 1, "x");
		x = luaL_checknumber(L, -1);
		lua_getfield(L, 1, "y");
		y = luaL_checknumber(L, -1);
		lua_getfield(L, 1, "z");
		z = luaL_checknumber(L, -1);
		lua_getfield(L, 1, "w");
		w = luaL_checknumber(L, -1);
	} while (0);
	p = calloc(1, sizeof(*p));
	kmVec4Fill(p, x, y, z, w);
	KAZMATH_LUA_BIND_META(L, kmVec4, p, KAZMATH_CLS_KMVEC4);
	return 1;
}

static int lua__kmVec4_gc(lua_State *L)
{
	kmVec4 *p = KAZMATH_CHECK_KMVEC4(L, 1);
	free(p);
	return 0;
}

static int lua__kmVec4_newindex(lua_State *L)
{
	kmVec4 *ptr = KAZMATH_CHECK_KMVEC4(L, 1);
	KAZMATH_LUA_NEWINDEX_FILED(L, KAZMATH_KMVEC4_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmVec4_index(lua_State *L)
{
	kmVec4 * ptr = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	KAZMATH_LUA_INDEX_METHOD(L, KAZMATH_CLS_KMVEC4);
	KAZMATH_LUA_INDEX_FIELD(L, KAZMATH_KMVEC4_FIELD_MAP, ptr);
	return 0;
}

static int lua__kmVec4ToArray(lua_State *L)
{
	kmVec4 * pOut = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	VEC4_TO_ARRAY(L, pOut);
	return 1;
}

static int lua__kmVec4ToTbl(lua_State *L)
{
	kmVec4 * pvec = (kmVec4 *)KAZMATH_CHECK_KMVEC4(L, 1);
	lua_newtable(L);
	lua_pushnumber(L, pvec->x);
	lua_setfield(L, -2, "x");
	lua_pushnumber(L, pvec->y);
	lua_setfield(L, -2, "y");
	lua_pushnumber(L, pvec->z);
	lua_setfield(L, -2, "z");
	lua_pushnumber(L, pvec->w);
	lua_setfield(L, -2, "w");
	return 1;
}

static int lua__kmAABB3New(lua_State *L)
{
	kmAABB3 *p = calloc(1, sizeof(*p));
	KAZMATH_LUA_BIND_META(L, kmAABB3, p, KAZMATH_CLS_KMAABB3);
	return 1;
}

static int lua__kmAABB3_gc(lua_State *L)
{
	kmAABB3 *p = KAZMATH_CHECK_KMAABB3(L, 1);
	free(p);
	return 0;
}

static int opencls__kmMat3(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Adjugate", lua__kmMat3Adjugate},
		{"AreEqual", lua__kmMat3AreEqual},
		{"AssignMat3", lua__kmMat3AssignMat3},
		{"Determinant", lua__kmMat3Determinant},
		{"ExtractForwardVec3", lua__kmMat3ExtractForwardVec3},
		{"ExtractRightVec3", lua__kmMat3ExtractRightVec3},
		{"ExtractRotationAxisAngle", lua__kmMat3ExtractRotationAxisAngle},
		{"ExtractUpVec3", lua__kmMat3ExtractUpVec3},
		{"Fill", lua__kmMat3Fill},
		{"FromRotationAxisAngle", lua__kmMat3FromRotationAxisAngle},
		{"FromRotationAxisAngleInDegrees", lua__kmMat3FromRotationAxisAngleInDegrees},
		{"FromRotationLookAt", lua__kmMat3FromRotationLookAt},
		{"FromRotationQuaternion", lua__kmMat3FromRotationQuaternion},
		{"FromRotationX", lua__kmMat3FromRotationX},
		{"FromRotationXInDegrees", lua__kmMat3FromRotationXInDegrees},
		{"FromRotationY", lua__kmMat3FromRotationY},
		{"FromRotationYInDegrees", lua__kmMat3FromRotationYInDegrees},
		{"FromRotationZ", lua__kmMat3FromRotationZ},
		{"FromRotationZInDegrees", lua__kmMat3FromRotationZInDegrees},
		{"FromScaling", lua__kmMat3FromScaling},
		{"FromTranslation", lua__kmMat3FromTranslation},
		{"Identity", lua__kmMat3Identity},
		{"Inverse", lua__kmMat3Inverse},
		{"IsIdentity", lua__kmMat3IsIdentity},
		{"MultiplyMat3", lua__kmMat3MultiplyMat3},
		{"MultiplyScalar", lua__kmMat3MultiplyScalar},
		{"Transpose", lua__kmMat3Transpose},
		{"ToArray", lua__kmMat3ToArray},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMMAT3);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmMat3_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}


static int opencls__kmRay2(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Fill", lua__kmRay2Fill},
		{"FillWithEndpoints", lua__kmRay2FillWithEndpoints},
		{"IntersectBox", lua__kmRay2IntersectBox},
		{"IntersectCircle", lua__kmRay2IntersectCircle},
		{"IntersectLineSegment", lua__kmRay2IntersectLineSegment},
		{"IntersectTriangle", lua__kmRay2IntersectTriangle},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMRAY2);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__method");
	lua_pushcfunction(L, lua__kmRay2_index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmRay2_newindex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, lua__kmRay2_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}


static int opencls__kmRay3(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Fill", lua__kmRay3Fill},
		{"FromPointAndDirection", lua__kmRay3FromPointAndDirection},
		{"IntersectPlane", lua__kmRay3IntersectPlane},
		{"IntersectTriangle", lua__kmRay3IntersectTriangle},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMRAY3);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__method");
	lua_pushcfunction(L, lua__kmRay3_index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmRay3_newindex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, lua__kmRay3_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}

static int opencls__kmQuaternion(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Add", lua__kmQuaternionAdd},
		{"AreEqual", lua__kmQuaternionAreEqual},
		{"Assign", lua__kmQuaternionAssign},
		{"BetweenVec3", lua__kmQuaternionBetweenVec3},
		{"Dot", lua__kmQuaternionDot},
		{"Exp", lua__kmQuaternionExp},
		{"ExtractRotationAroundAxis", lua__kmQuaternionExtractRotationAroundAxis},
		{"Fill", lua__kmQuaternionFill},
		{"GetForwardVec3LH", lua__kmQuaternionGetForwardVec3LH},
		{"GetForwardVec3RH", lua__kmQuaternionGetForwardVec3RH},
		{"GetPitch", lua__kmQuaternionGetPitch},
		{"GetRightVec3", lua__kmQuaternionGetRightVec3},
		{"GetRoll", lua__kmQuaternionGetRoll},
		{"GetUpVec3", lua__kmQuaternionGetUpVec3},
		{"GetYaw", lua__kmQuaternionGetYaw},
		{"Identity", lua__kmQuaternionIdentity},
		{"Inverse", lua__kmQuaternionInverse},
		{"IsIdentity", lua__kmQuaternionIsIdentity},
		{"Length", lua__kmQuaternionLength},
		{"LengthSq", lua__kmQuaternionLengthSq},
		{"Ln", lua__kmQuaternionLn},
		{"LookRotation", lua__kmQuaternionLookRotation},
		{"Multiply", lua__kmQuaternionMultiply},
		{"MultiplyVec3", lua__kmQuaternionMultiplyVec3},
		{"Normalize", lua__kmQuaternionNormalize},
		{"RotationAxisAngle", lua__kmQuaternionRotationAxisAngle},
		{"RotationBetweenVec3", lua__kmQuaternionRotationBetweenVec3},
		{"RotationMatrix", lua__kmQuaternionRotationMatrix},
		{"RotationPitchYawRoll", lua__kmQuaternionRotationPitchYawRoll},
		{"Scale", lua__kmQuaternionScale},
		{"Slerp", lua__kmQuaternionSlerp},
		{"Subtract", lua__kmQuaternionSubtract},
		{"ToAxisAngle", lua__kmQuaternionToAxisAngle},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMQUATERNION);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__method");
	lua_pushcfunction(L, lua__kmQuaternion_index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmQuaternion_newindex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, lua__kmQuaternion_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}

static int opencls__kmMat4(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"AreEqual", lua__kmMat4AreEqual},
		{"Assign", lua__kmMat4Assign},
		{"AssignMat3", lua__kmMat4AssignMat3},
		{"ExtractPlane", lua__kmMat4ExtractPlane},
		{"ExtractRotationMat3", lua__kmMat4ExtractRotationMat3},
		{"ExtractTranslationVec3", lua__kmMat4ExtractTranslationVec3},
		{"Fill", lua__kmMat4Fill},
		{"GetForwardVec3LH", lua__kmMat4GetForwardVec3LH},
		{"GetForwardVec3RH", lua__kmMat4GetForwardVec3RH},
		{"GetRightVec3", lua__kmMat4GetRightVec3},
		{"GetUpVec3", lua__kmMat4GetUpVec3},
		{"Identity", lua__kmMat4Identity},
		{"Inverse", lua__kmMat4Inverse},
		{"IsIdentity", lua__kmMat4IsIdentity},
		{"LookAt", lua__kmMat4LookAt},
		{"Multiply", lua__kmMat4Multiply},
		{"OrthographicProjection", lua__kmMat4OrthographicProjection},
		{"PerspectiveProjection", lua__kmMat4PerspectiveProjection},
		{"RotationAxisAngle", lua__kmMat4RotationAxisAngle},
		{"RotationQuaternion", lua__kmMat4RotationQuaternion},
		{"RotationToAxisAngle", lua__kmMat4RotationToAxisAngle},
		{"RotationTranslation", lua__kmMat4RotationTranslation},
		{"RotationX", lua__kmMat4RotationX},
		{"RotationY", lua__kmMat4RotationY},
		{"RotationYawPitchRoll", lua__kmMat4RotationYawPitchRoll},
		{"RotationZ", lua__kmMat4RotationZ},
		{"Scaling", lua__kmMat4Scaling},
		{"Translation", lua__kmMat4Translation},
		{"Transpose", lua__kmMat4Transpose},
		{"ToArray", lua__kmMat4ToArray},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMMAT4);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmMat4_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}


static int opencls__kmAABB2(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Assign", lua__kmAABB2Assign},
		{"Centre", lua__kmAABB2Centre},
		{"ContainsAABB", lua__kmAABB2ContainsAABB},
		{"ContainsPoint", lua__kmAABB2ContainsPoint},
		{"DiameterX", lua__kmAABB2DiameterX},
		{"DiameterY", lua__kmAABB2DiameterY},
		{"ExpandToContain", lua__kmAABB2ExpandToContain},
		{"Initialize", lua__kmAABB2Initialize},
		{"Sanitize", lua__kmAABB2Sanitize},
		{"Scale", lua__kmAABB2Scale},
		{"ScaleWithPivot", lua__kmAABB2ScaleWithPivot},
		{"Translate", lua__kmAABB2Translate},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMAABB2);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmAABB2_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}


static int opencls__kmPlane(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"ClassifyPoint", lua__kmPlaneClassifyPoint},
		{"Dot", lua__kmPlaneDot},
		{"DotCoord", lua__kmPlaneDotCoord},
		{"DotNormal", lua__kmPlaneDotNormal},
		{"ExtractFromMat4", lua__kmPlaneExtractFromMat4},
		{"Fill", lua__kmPlaneFill},
		{"FromNormalAndDistance", lua__kmPlaneFromNormalAndDistance},
		{"FromPointAndNormal", lua__kmPlaneFromPointAndNormal},
		{"FromPoints", lua__kmPlaneFromPoints},
		{"GetIntersection", lua__kmPlaneGetIntersection},
		{"IntersectLine", lua__kmPlaneIntersectLine},
		{"Normalize", lua__kmPlaneNormalize},
		{"Scale", lua__kmPlaneScale},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMPLANE);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__method");
	lua_pushcfunction(L, lua__kmPlane_index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmPlane_newindex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, lua__kmPlane_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}


static int opencls__kmVec2(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Add", lua__kmVec2Add},
		{"AreEqual", lua__kmVec2AreEqual},
		{"Assign", lua__kmVec2Assign},
		{"Cross", lua__kmVec2Cross},
		{"DegreesBetween", lua__kmVec2DegreesBetween},
		{"DistanceBetween", lua__kmVec2DistanceBetween},
		{"Div", lua__kmVec2Div},
		{"Dot", lua__kmVec2Dot},
		{"Fill", lua__kmVec2Fill},
		{"Length", lua__kmVec2Length},
		{"LengthSq", lua__kmVec2LengthSq},
		{"Lerp", lua__kmVec2Lerp},
		{"MidPointBetween", lua__kmVec2MidPointBetween},
		{"Mul", lua__kmVec2Mul},
		{"Normalize", lua__kmVec2Normalize},
		{"Reflect", lua__kmVec2Reflect},
		{"RotateBy", lua__kmVec2RotateBy},
		{"Scale", lua__kmVec2Scale},
		{"Subtract", lua__kmVec2Subtract},
		{"Swap", lua__kmVec2Swap},
		{"Transform", lua__kmVec2Transform},
		{"TransformCoord", lua__kmVec2TransformCoord},
		{"ToArray", lua__kmVec2ToArray},
		{"ToTbl", lua__kmVec2ToTbl},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMVEC2);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__method");
	lua_pushcfunction(L, lua__kmVec2_index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmVec2_newindex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, lua__kmVec2_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}


static int opencls__kmVec3(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Add", lua__kmVec3Add},
		{"AreEqual", lua__kmVec3AreEqual},
		{"Assign", lua__kmVec3Assign},
		{"Cross", lua__kmVec3Cross},
		{"Div", lua__kmVec3Div},
		{"Dot", lua__kmVec3Dot},
		{"Fill", lua__kmVec3Fill},
		{"GetHorizontalAngle", lua__kmVec3GetHorizontalAngle},
		{"InverseTransform", lua__kmVec3InverseTransform},
		{"InverseTransformNormal", lua__kmVec3InverseTransformNormal},
		{"Length", lua__kmVec3Length},
		{"LengthSq", lua__kmVec3LengthSq},
		{"Lerp", lua__kmVec3Lerp},
		{"Mul", lua__kmVec3Mul},
		{"MultiplyMat3", lua__kmVec3MultiplyMat3},
		{"MultiplyMat4", lua__kmVec3MultiplyMat4},
		{"Normalize", lua__kmVec3Normalize},
		{"OrthoNormalize", lua__kmVec3OrthoNormalize},
		{"ProjectOnToPlane", lua__kmVec3ProjectOnToPlane},
		{"ProjectOnToVec3", lua__kmVec3ProjectOnToVec3},
		{"Reflect", lua__kmVec3Reflect},
		{"RotationToDirection", lua__kmVec3RotationToDirection},
		{"Scale", lua__kmVec3Scale},
		{"Subtract", lua__kmVec3Subtract},
		{"Swap", lua__kmVec3Swap},
		{"Transform", lua__kmVec3Transform},
		{"TransformCoord", lua__kmVec3TransformCoord},
		{"TransformNormal", lua__kmVec3TransformNormal},
		{"Zero", lua__kmVec3Zero},
		{"ToArray", lua__kmVec3ToArray},
		{"ToTbl", lua__kmVec3ToTbl},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMVEC3);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__method");
	lua_pushcfunction(L, lua__kmVec3_index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmVec3_newindex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, lua__kmVec3_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}


static int opencls__kmVec4(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Add", lua__kmVec4Add},
		{"AreEqual", lua__kmVec4AreEqual},
		{"Assign", lua__kmVec4Assign},
		{"Div", lua__kmVec4Div},
		{"Dot", lua__kmVec4Dot},
		{"Fill", lua__kmVec4Fill},
		{"Length", lua__kmVec4Length},
		{"LengthSq", lua__kmVec4LengthSq},
		{"Lerp", lua__kmVec4Lerp},
		{"Mul", lua__kmVec4Mul},
		{"MultiplyMat4", lua__kmVec4MultiplyMat4},
		{"Normalize", lua__kmVec4Normalize},
		{"Scale", lua__kmVec4Scale},
		{"Subtract", lua__kmVec4Subtract},
		{"Swap", lua__kmVec4Swap},
		{"Transform", lua__kmVec4Transform},
		{"TransformArray", lua__kmVec4TransformArray},
		{"ToArray", lua__kmVec4ToArray},
		{"ToTbl", lua__kmVec4ToTbl},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMVEC4);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__method");
	lua_pushcfunction(L, lua__kmVec4_index);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmVec4_newindex);
	lua_setfield(L, -2, "__newindex");
	lua_pushcfunction(L, lua__kmVec4_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}

static int opencls__kmAABB3(lua_State *L)
{
	luaL_Reg lmethods[] = {
		{"Assign", lua__kmAABB3Assign},
		{"Centre", lua__kmAABB3Centre},
		{"ContainsAABB", lua__kmAABB3ContainsAABB},
		{"ContainsPoint", lua__kmAABB3ContainsPoint},
		{"DiameterX", lua__kmAABB3DiameterX},
		{"DiameterY", lua__kmAABB3DiameterY},
		{"DiameterZ", lua__kmAABB3DiameterZ},
		{"ExpandToContain", lua__kmAABB3ExpandToContain},
		{"Initialize", lua__kmAABB3Initialize},
		{"IntersectsAABB", lua__kmAABB3IntersectsAABB},
		{"IntersectsTriangle", lua__kmAABB3IntersectsTriangle},
		{"Scale", lua__kmAABB3Scale},
		{NULL, NULL},
	};
	luaL_newmetatable(L, KAZMATH_CLS_KMAABB3);
	lua_newtable(L);
	luaL_register(L, NULL, lmethods);
	lua_setfield(L, -2, "__index");
	lua_pushcfunction(L, lua__kmAABB3_gc);
	lua_setfield(L, -2, "__gc");
	return 1;
}

int luaopen_lkazmath(lua_State *L)
{
	luaL_Reg lfuncs[] = {
		/* Utility */
		{"kmAlmostEqual", lua__kmAlmostEqual},
		{"kmClamp", lua__kmClamp},
		{"kmDegreesToRadians", lua__kmDegreesToRadians},
		{"kmLerp", lua__kmLerp},
		{"kmLine2WithLineIntersection", lua__kmLine2WithLineIntersection},
		{"kmRadiansToDegrees", lua__kmRadiansToDegrees},
		{"kmSQR", lua__kmSQR},
		{"kmSegment2WithSegmentIntersection", lua__kmSegment2WithSegmentIntersection},
		{"kmMax", lua__kmMax},
		{"kmMin", lua__kmMin},

		/* new */
		{"kmMat3New", lua__kmMat3New},
		{"kmMat3NewWithArray", lua__kmMat3NewWithArray},
		{"kmRay2New", lua__kmRay2New},
		{"kmRay3New", lua__kmRay3New},
		{"kmQuaternionNew", lua__kmQuaternionNew},
		{"kmMat4New", lua__kmMat4New},
		{"kmMat4NewWithArray", lua__kmMat4NewWithArray},
		{"kmAABB2New", lua__kmAABB2New},
		{"kmPlaneNew", lua__kmPlaneNew},
		{"kmVec2New", lua__kmVec2New},
		{"kmVec2NewWithTbl", lua__kmVec2NewWithTbl},
		{"kmVec3New", lua__kmVec3New},
		{"kmVec3NewWithTbl", lua__kmVec3NewWithTbl},
		{"kmVec4New", lua__kmVec4New},
		{"kmVec4NewWithTbl", lua__kmVec4NewWithTbl},
		{"kmAABB3New", lua__kmAABB3New},

		/* AABB2 */
		{"kmAABB2Assign", lua__kmAABB2Assign},
		{"kmAABB2Centre", lua__kmAABB2Centre},
		{"kmAABB2ContainsAABB", lua__kmAABB2ContainsAABB},
		{"kmAABB2ContainsPoint", lua__kmAABB2ContainsPoint},
		{"kmAABB2DiameterX", lua__kmAABB2DiameterX},
		{"kmAABB2DiameterY", lua__kmAABB2DiameterY},
		{"kmAABB2ExpandToContain", lua__kmAABB2ExpandToContain},
		{"kmAABB2Initialize", lua__kmAABB2Initialize},
		{"kmAABB2Sanitize", lua__kmAABB2Sanitize},
		{"kmAABB2Scale", lua__kmAABB2Scale},
		{"kmAABB2ScaleWithPivot", lua__kmAABB2ScaleWithPivot},
		{"kmAABB2Translate", lua__kmAABB2Translate},

		/* AABB3 */
		{"kmAABB3Assign", lua__kmAABB3Assign},
		{"kmAABB3Centre", lua__kmAABB3Centre},
		{"kmAABB3ContainsAABB", lua__kmAABB3ContainsAABB},
		{"kmAABB3ContainsPoint", lua__kmAABB3ContainsPoint},
		{"kmAABB3DiameterX", lua__kmAABB3DiameterX},
		{"kmAABB3DiameterY", lua__kmAABB3DiameterY},
		{"kmAABB3DiameterZ", lua__kmAABB3DiameterZ},
		{"kmAABB3ExpandToContain", lua__kmAABB3ExpandToContain},
		{"kmAABB3Initialize", lua__kmAABB3Initialize},
		{"kmAABB3IntersectsAABB", lua__kmAABB3IntersectsAABB},
		{"kmAABB3IntersectsTriangle", lua__kmAABB3IntersectsTriangle},
		{"kmAABB3Scale", lua__kmAABB3Scale},

		/* Mat3 */
		{"kmMat3Adjugate", lua__kmMat3Adjugate},
		{"kmMat3AreEqual", lua__kmMat3AreEqual},
		{"kmMat3AssignMat3", lua__kmMat3AssignMat3},
		{"kmMat3Determinant", lua__kmMat3Determinant},
		{"kmMat3ExtractForwardVec3", lua__kmMat3ExtractForwardVec3},
		{"kmMat3ExtractRightVec3", lua__kmMat3ExtractRightVec3},
		{"kmMat3ExtractRotationAxisAngle", lua__kmMat3ExtractRotationAxisAngle},
		{"kmMat3ExtractUpVec3", lua__kmMat3ExtractUpVec3},
		{"kmMat3Fill", lua__kmMat3Fill},
		{"kmMat3FromRotationAxisAngle", lua__kmMat3FromRotationAxisAngle},
		{"kmMat3FromRotationAxisAngleInDegrees", lua__kmMat3FromRotationAxisAngleInDegrees},
		{"kmMat3FromRotationLookAt", lua__kmMat3FromRotationLookAt},
		{"kmMat3FromRotationQuaternion", lua__kmMat3FromRotationQuaternion},
		{"kmMat3FromRotationX", lua__kmMat3FromRotationX},
		{"kmMat3FromRotationXInDegrees", lua__kmMat3FromRotationXInDegrees},
		{"kmMat3FromRotationY", lua__kmMat3FromRotationY},
		{"kmMat3FromRotationYInDegrees", lua__kmMat3FromRotationYInDegrees},
		{"kmMat3FromRotationZ", lua__kmMat3FromRotationZ},
		{"kmMat3FromRotationZInDegrees", lua__kmMat3FromRotationZInDegrees},
		{"kmMat3FromScaling", lua__kmMat3FromScaling},
		{"kmMat3FromTranslation", lua__kmMat3FromTranslation},
		{"kmMat3Identity", lua__kmMat3Identity},
		{"kmMat3Inverse", lua__kmMat3Inverse},
		{"kmMat3IsIdentity", lua__kmMat3IsIdentity},
		{"kmMat3MultiplyMat3", lua__kmMat3MultiplyMat3},
		{"kmMat3MultiplyScalar", lua__kmMat3MultiplyScalar},
		{"kmMat3Transpose", lua__kmMat3Transpose},

		/* Mat4 */
		{"kmMat4AreEqual", lua__kmMat4AreEqual},
		{"kmMat4Assign", lua__kmMat4Assign},
		{"kmMat4AssignMat3", lua__kmMat4AssignMat3},
		{"kmMat4ExtractPlane", lua__kmMat4ExtractPlane},
		{"kmMat4ExtractRotationMat3", lua__kmMat4ExtractRotationMat3},
		{"kmMat4ExtractTranslationVec3", lua__kmMat4ExtractTranslationVec3},
		{"kmMat4Fill", lua__kmMat4Fill},
		{"kmMat4GetForwardVec3LH", lua__kmMat4GetForwardVec3LH},
		{"kmMat4GetForwardVec3RH", lua__kmMat4GetForwardVec3RH},
		{"kmMat4GetRightVec3", lua__kmMat4GetRightVec3},
		{"kmMat4GetUpVec3", lua__kmMat4GetUpVec3},
		{"kmMat4Identity", lua__kmMat4Identity},
		{"kmMat4Inverse", lua__kmMat4Inverse},
		{"kmMat4IsIdentity", lua__kmMat4IsIdentity},
		{"kmMat4LookAt", lua__kmMat4LookAt},
		{"kmMat4Multiply", lua__kmMat4Multiply},
		{"kmMat4OrthographicProjection", lua__kmMat4OrthographicProjection},
		{"kmMat4PerspectiveProjection", lua__kmMat4PerspectiveProjection},
		{"kmMat4RotationAxisAngle", lua__kmMat4RotationAxisAngle},
		{"kmMat4RotationQuaternion", lua__kmMat4RotationQuaternion},
		{"kmMat4RotationToAxisAngle", lua__kmMat4RotationToAxisAngle},
		{"kmMat4RotationTranslation", lua__kmMat4RotationTranslation},
		{"kmMat4RotationX", lua__kmMat4RotationX},
		{"kmMat4RotationY", lua__kmMat4RotationY},
		{"kmMat4RotationYawPitchRoll", lua__kmMat4RotationYawPitchRoll},
		{"kmMat4RotationZ", lua__kmMat4RotationZ},
		{"kmMat4Scaling", lua__kmMat4Scaling},
		{"kmMat4Translation", lua__kmMat4Translation},
		{"kmMat4Transpose", lua__kmMat4Transpose},

		/* Plane */
		{"kmPlaneClassifyPoint", lua__kmPlaneClassifyPoint},
		{"kmPlaneDot", lua__kmPlaneDot},
		{"kmPlaneDotCoord", lua__kmPlaneDotCoord},
		{"kmPlaneDotNormal", lua__kmPlaneDotNormal},
		{"kmPlaneExtractFromMat4", lua__kmPlaneExtractFromMat4},
		{"kmPlaneFill", lua__kmPlaneFill},
		{"kmPlaneFromNormalAndDistance", lua__kmPlaneFromNormalAndDistance},
		{"kmPlaneFromPointAndNormal", lua__kmPlaneFromPointAndNormal},
		{"kmPlaneFromPoints", lua__kmPlaneFromPoints},
		{"kmPlaneGetIntersection", lua__kmPlaneGetIntersection},
		{"kmPlaneIntersectLine", lua__kmPlaneIntersectLine},
		{"kmPlaneNormalize", lua__kmPlaneNormalize},
		{"kmPlaneScale", lua__kmPlaneScale},

		/* Quaternion */
		{"kmQuaternionAdd", lua__kmQuaternionAdd},
		{"kmQuaternionAreEqual", lua__kmQuaternionAreEqual},
		{"kmQuaternionAssign", lua__kmQuaternionAssign},
		{"kmQuaternionBetweenVec3", lua__kmQuaternionBetweenVec3},
		{"kmQuaternionDot", lua__kmQuaternionDot},
		{"kmQuaternionExp", lua__kmQuaternionExp},
		{"kmQuaternionExtractRotationAroundAxis", lua__kmQuaternionExtractRotationAroundAxis},
		{"kmQuaternionFill", lua__kmQuaternionFill},
		{"kmQuaternionGetForwardVec3LH", lua__kmQuaternionGetForwardVec3LH},
		{"kmQuaternionGetForwardVec3RH", lua__kmQuaternionGetForwardVec3RH},
		{"kmQuaternionGetPitch", lua__kmQuaternionGetPitch},
		{"kmQuaternionGetRightVec3", lua__kmQuaternionGetRightVec3},
		{"kmQuaternionGetRoll", lua__kmQuaternionGetRoll},
		{"kmQuaternionGetUpVec3", lua__kmQuaternionGetUpVec3},
		{"kmQuaternionGetYaw", lua__kmQuaternionGetYaw},
		{"kmQuaternionIdentity", lua__kmQuaternionIdentity},
		{"kmQuaternionInverse", lua__kmQuaternionInverse},
		{"kmQuaternionIsIdentity", lua__kmQuaternionIsIdentity},
		{"kmQuaternionLength", lua__kmQuaternionLength},
		{"kmQuaternionLengthSq", lua__kmQuaternionLengthSq},
		{"kmQuaternionLn", lua__kmQuaternionLn},
		{"kmQuaternionLookRotation", lua__kmQuaternionLookRotation},
		{"kmQuaternionMultiply", lua__kmQuaternionMultiply},
		{"kmQuaternionMultiplyVec3", lua__kmQuaternionMultiplyVec3},
		{"kmQuaternionNormalize", lua__kmQuaternionNormalize},
		{"kmQuaternionRotationAxisAngle", lua__kmQuaternionRotationAxisAngle},
		{"kmQuaternionRotationBetweenVec3", lua__kmQuaternionRotationBetweenVec3},
		{"kmQuaternionRotationMatrix", lua__kmQuaternionRotationMatrix},
		{"kmQuaternionRotationPitchYawRoll", lua__kmQuaternionRotationPitchYawRoll},
		{"kmQuaternionScale", lua__kmQuaternionScale},
		{"kmQuaternionSlerp", lua__kmQuaternionSlerp},
		{"kmQuaternionSubtract", lua__kmQuaternionSubtract},
		{"kmQuaternionToAxisAngle", lua__kmQuaternionToAxisAngle},

		/* Ray2 */
		{"kmRay2Fill", lua__kmRay2Fill},
		{"kmRay2FillWithEndpoints", lua__kmRay2FillWithEndpoints},
		{"kmRay2IntersectBox", lua__kmRay2IntersectBox},
		{"kmRay2IntersectCircle", lua__kmRay2IntersectCircle},
		{"kmRay2IntersectLineSegment", lua__kmRay2IntersectLineSegment},
		{"kmRay2IntersectTriangle", lua__kmRay2IntersectTriangle},

		/* Ray3 */
		{"kmRay3Fill", lua__kmRay3Fill},
		{"kmRay3FromPointAndDirection", lua__kmRay3FromPointAndDirection},
		{"kmRay3IntersectPlane", lua__kmRay3IntersectPlane},
		{"kmRay3IntersectTriangle", lua__kmRay3IntersectTriangle},

		/* Vec2 */
		{"kmVec2Add", lua__kmVec2Add},
		{"kmVec2AreEqual", lua__kmVec2AreEqual},
		{"kmVec2Assign", lua__kmVec2Assign},
		{"kmVec2Cross", lua__kmVec2Cross},
		{"kmVec2DegreesBetween", lua__kmVec2DegreesBetween},
		{"kmVec2DistanceBetween", lua__kmVec2DistanceBetween},
		{"kmVec2Div", lua__kmVec2Div},
		{"kmVec2Dot", lua__kmVec2Dot},
		{"kmVec2Fill", lua__kmVec2Fill},
		{"kmVec2Length", lua__kmVec2Length},
		{"kmVec2LengthSq", lua__kmVec2LengthSq},
		{"kmVec2Lerp", lua__kmVec2Lerp},
		{"kmVec2MidPointBetween", lua__kmVec2MidPointBetween},
		{"kmVec2Mul", lua__kmVec2Mul},
		{"kmVec2Normalize", lua__kmVec2Normalize},
		{"kmVec2Reflect", lua__kmVec2Reflect},
		{"kmVec2RotateBy", lua__kmVec2RotateBy},
		{"kmVec2Scale", lua__kmVec2Scale},
		{"kmVec2Subtract", lua__kmVec2Subtract},
		{"kmVec2Swap", lua__kmVec2Swap},
		{"kmVec2Transform", lua__kmVec2Transform},
		{"kmVec2TransformCoord", lua__kmVec2TransformCoord},

		/* Vec3 */
		{"kmVec3Add", lua__kmVec3Add},
		{"kmVec3AreEqual", lua__kmVec3AreEqual},
		{"kmVec3Assign", lua__kmVec3Assign},
		{"kmVec3Cross", lua__kmVec3Cross},
		{"kmVec3Div", lua__kmVec3Div},
		{"kmVec3Dot", lua__kmVec3Dot},
		{"kmVec3Fill", lua__kmVec3Fill},
		{"kmVec3GetHorizontalAngle", lua__kmVec3GetHorizontalAngle},
		{"kmVec3InverseTransform", lua__kmVec3InverseTransform},
		{"kmVec3InverseTransformNormal", lua__kmVec3InverseTransformNormal},
		{"kmVec3Length", lua__kmVec3Length},
		{"kmVec3LengthSq", lua__kmVec3LengthSq},
		{"kmVec3Lerp", lua__kmVec3Lerp},
		{"kmVec3Mul", lua__kmVec3Mul},
		{"kmVec3MultiplyMat3", lua__kmVec3MultiplyMat3},
		{"kmVec3MultiplyMat4", lua__kmVec3MultiplyMat4},
		{"kmVec3Normalize", lua__kmVec3Normalize},
		{"kmVec3OrthoNormalize", lua__kmVec3OrthoNormalize},
		{"kmVec3ProjectOnToPlane", lua__kmVec3ProjectOnToPlane},
		{"kmVec3ProjectOnToVec3", lua__kmVec3ProjectOnToVec3},
		{"kmVec3Reflect", lua__kmVec3Reflect},
		{"kmVec3RotationToDirection", lua__kmVec3RotationToDirection},
		{"kmVec3Scale", lua__kmVec3Scale},
		{"kmVec3Subtract", lua__kmVec3Subtract},
		{"kmVec3Swap", lua__kmVec3Swap},
		{"kmVec3Transform", lua__kmVec3Transform},
		{"kmVec3TransformCoord", lua__kmVec3TransformCoord},
		{"kmVec3TransformNormal", lua__kmVec3TransformNormal},
		{"kmVec3Zero", lua__kmVec3Zero},

		/* Vec4 */
		{"kmVec4Add", lua__kmVec4Add},
		{"kmVec4AreEqual", lua__kmVec4AreEqual},
		{"kmVec4Assign", lua__kmVec4Assign},
		{"kmVec4Div", lua__kmVec4Div},
		{"kmVec4Dot", lua__kmVec4Dot},
		{"kmVec4Fill", lua__kmVec4Fill},
		{"kmVec4Length", lua__kmVec4Length},
		{"kmVec4LengthSq", lua__kmVec4LengthSq},
		{"kmVec4Lerp", lua__kmVec4Lerp},
		{"kmVec4Mul", lua__kmVec4Mul},
		{"kmVec4MultiplyMat4", lua__kmVec4MultiplyMat4},
		{"kmVec4Normalize", lua__kmVec4Normalize},
		{"kmVec4Scale", lua__kmVec4Scale},
		{"kmVec4Subtract", lua__kmVec4Subtract},
		{"kmVec4Swap", lua__kmVec4Swap},
		{"kmVec4Transform", lua__kmVec4Transform},
		{"kmVec4TransformArray", lua__kmVec4TransformArray},

		{"kmVec2ToArray", lua__kmVec2ToArray},
		{"kmVec3ToArray", lua__kmVec3ToArray},
		{"kmVec4ToArray", lua__kmVec4ToArray},

		{"kmMat3ToArray", lua__kmMat3ToArray},
		{"kmMat4ToArray", lua__kmMat4ToArray},

		{"kmVec2ToTbl", lua__kmVec2ToTbl},
		{"kmVec3ToTbl", lua__kmVec3ToTbl},
		{"kmVec4ToTbl", lua__kmVec4ToTbl},


		{NULL, NULL}
	};
	opencls__kmMat3(L);
	opencls__kmRay2(L);
	opencls__kmRay3(L);
	opencls__kmQuaternion(L);
	opencls__kmMat4(L);
	opencls__kmAABB2(L);
	opencls__kmPlane(L);
	opencls__kmVec2(L);
	opencls__kmVec3(L);
	opencls__kmVec4(L);
	opencls__kmAABB3(L);
	luaL_newlib(L, lfuncs);

	do {
		/* see kmAABB2ContainsAABB, kmAABB3ContainsAABB */
		KAZMATH_LUA_SETMACRO(L, -1, "KM_CONTAINS_NONE", KM_CONTAINS_NONE);
		KAZMATH_LUA_SETMACRO(L, -1, "KM_CONTAINS_PARTIAL", KM_CONTAINS_PARTIAL);
		KAZMATH_LUA_SETMACRO(L, -1, "KM_CONTAINS_ALL", KM_CONTAINS_ALL);

		/* see kmMat4ExtractPlane, param plane */
		KAZMATH_LUA_SETMACRO(L, -1, "KM_PLANE_LEFT", KM_PLANE_LEFT);
		KAZMATH_LUA_SETMACRO(L, -1, "KM_PLANE_RIGHT", KM_PLANE_RIGHT);
		KAZMATH_LUA_SETMACRO(L, -1, "KM_PLANE_BOTTOM", KM_PLANE_BOTTOM);
		KAZMATH_LUA_SETMACRO(L, -1, "KM_PLANE_TOP", KM_PLANE_TOP);
		KAZMATH_LUA_SETMACRO(L, -1, "KM_PLANE_NEAR", KM_PLANE_NEAR);
		KAZMATH_LUA_SETMACRO(L, -1, "KM_PLANE_FAR", KM_PLANE_FAR);

		/* see kmPlaneClassifyPoint */
		KAZMATH_LUA_SETMACRO(L, -1, "POINT_BEHIND_PLANE", POINT_BEHIND_PLANE);
		KAZMATH_LUA_SETMACRO(L, -1, "POINT_ON_PLANE", POINT_ON_PLANE);
		KAZMATH_LUA_SETMACRO(L, -1, "POINT_INFRONT_OF_PLANE", POINT_INFRONT_OF_PLANE);

	} while (0);

	return 1;
}
