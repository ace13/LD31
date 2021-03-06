#include "Rect.hpp"
#include "Common.hpp"

#include <Script/ScriptExtensions.hpp>
#include <angelscript.h>

#include <cassert>

using namespace Math;

Rect::Rect() :
    Top(0), Left(0), Width(0), Height(0)
{
}
Rect::Rect(float left, float top, float width, float height)
{
    if (width < 0)
    {
        left = left - width;
        width = -width;
    }
    if (height < 0)
    {
        top = top - height;
        height = -height;
    }

    Top = top;
    Left = left;
    Width = width;
    Height = height;
}
Rect::Rect(const Vector2& topleft, float width, float height)
{
    float top = topleft.Y, left = topleft.X;

    if (width < 0)
    {
        left = left - width;
        width = -width;
    }
    if (height < 0)
    {
        top = top - height;
        height = -height;
    }

    Top = top;
    Left = left;
    Width = width;
    Height = height;
}
Rect::Rect(const Vector2& topleft, const Vector2& bottomright) :
    Top(topleft.Y), Left(topleft.X), Width(bottomright.X - topleft.X), Height(bottomright.Y - topleft.Y)
{
}

bool Rect::operator==(const Rect& rhs) const
{
    return FloatCompare(Top, rhs.Top, 0.0000001f) && FloatCompare(Left, rhs.Left, 0.0000001f) && FloatCompare(Width, rhs.Width, 0.0000001f) && FloatCompare(Height, rhs.Height, 0.0000001f);
}

Vector2 Rect::getTopLeft() const
{
    return Vector2(Left, Top);
}
Vector2 Rect::getBottomRight() const
{
    return Vector2(Left + Width, Top + Height);
}
Vector2 Rect::getCenter() const
{
    return Vector2(Left + Width / 2.f, Top + Height / 2.f);
}
Vector2 Rect::getSize() const
{
    return Vector2(Width, Height);
}

Vector2 Rect::constrain(const Vector2& point)
{
    return Vector2(point.X < Left ? Left : (point.X > Left + Width ? Left + Width : point.X), point.Y < Top ? Top : (point.Y > Top + Height ? Top + Height : point.Y));
}

bool Rect::contains(const Vector2& point)
{
    return Left <= point.X && Top <= point.Y && Left + Width >= point.X && Top + Height >= point.Y;
}
bool Rect::intersects(const Rect& rect)
{
    float interLeft = std::max(Left, rect.Left),
        interTop = std::max(Top, rect.Top),
        interRight = std::min(Left + Width, rect.Left + rect.Width),
        interBottom = std::min(Top + Height, rect.Top + rect.Height);

    return (interLeft < interRight && interTop < interBottom);
}
bool Rect::intersects(const Rect& rect, Rect& intersecting)
{
    float interLeft = std::max(Left, rect.Left),
        interTop = std::max(Top, rect.Top),
        interRight = std::min(Left + Width, rect.Left + rect.Width),
        interBottom = std::min(Top + Height, rect.Top + rect.Height);

    if (interLeft < interRight && interTop < interBottom)
    {
        intersecting = Rect(interLeft, interTop, interRight - interLeft, interBottom - interTop);
        return true;
    }

    intersecting = Rect();
    return false;
}

namespace
{
    void create_rect(void* memory) {
        new(memory) Rect();
    }
    void create_rect_loose(float l, float t, float w, float h, void* memory) {
        new(memory) Rect(l, t, w, h);
    }
    void create_rect_tlwh(const Vector2& tl, float w, float h, void* memory) {
        new(memory) Rect(tl, w, h);
    }
    void create_rect_tlbr(const Vector2& tl, const Vector2& br, void* memory) {
        new(memory) Rect(tl, br);
    }
    void destruct_rect(Rect* memory) {
        memory->~Rect();
    }

    void rect_setBottomRight(const Vector2& br, Rect& rect)
    {
        auto tl = rect.getTopLeft();
        if (tl.X > br.X || tl.Y > br.Y)
            return;

        rect.Width = br.X - tl.X;
        rect.Height = br.Y - tl.Y;
    }

    void rect_setTopLeft(const Vector2& tl, Rect& rect)
    {
        auto br = rect.getBottomRight();
        if (tl.X > br.X || tl.Y > br.Y)
            return;

        rect.Top = tl.Y;
        rect.Left = tl.X;
        rect.Width = br.X - tl.X;
        rect.Height = br.Y - tl.Y;
    }

#ifndef AS_SUPPORT_VALRET
    void getCenter_rect(asIScriptGeneric* gen)
    {
        Math::Rect& rect = *reinterpret_cast<Math::Rect*>(gen->GetObject());
        new (gen->GetAddressOfReturnLocation()) Math::Vector2(rect.getCenter());
    }
    void topLeft_rect(asIScriptGeneric* gen)
    {
        Math::Rect& rect = *reinterpret_cast<Math::Rect*>(gen->GetObject());
        new (gen->GetAddressOfReturnLocation()) Math::Vector2(rect.getTopLeft());
    }
    void bottomRight_rect(asIScriptGeneric* gen)
    {
        Math::Rect& rect = *reinterpret_cast<Math::Rect*>(gen->GetObject());
        new (gen->GetAddressOfReturnLocation()) Math::Vector2(rect.getBottomRight());
    }
    void getSize_rect(asIScriptGeneric* gen)
    {
        Math::Rect& rect = *reinterpret_cast<Math::Rect*>(gen->GetObject());
        new (gen->GetAddressOfReturnLocation()) Math::Vector2(rect.getSize());
    }
    void constrain_rect(asIScriptGeneric* gen)
    {
        Math::Rect& rect = *reinterpret_cast<Math::Rect*>(gen->GetObject());
        Math::Vector2& vec = *reinterpret_cast<Math::Vector2*>(gen->GetArgObject(0));
        new (gen->GetAddressOfReturnLocation()) Math::Vector2(rect.constrain(vec));
    }
#endif

    bool Reg()
    {
        Script::ScriptExtensions::AddExtension([](asIScriptEngine* eng) {
            int r = 0;
            r = eng->RegisterObjectType("Rect", sizeof(Rect), asOBJ_VALUE | asGetTypeTraits<Rect>()); assert(r >= 0);

            r = eng->RegisterObjectBehaviour("Rect", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(create_rect), asCALL_CDECL_OBJLAST); assert(r >= 0);
            r = eng->RegisterObjectBehaviour("Rect", asBEHAVE_CONSTRUCT, "void f(float,float,float,float)", asFUNCTION(create_rect_loose), asCALL_CDECL_OBJLAST); assert(r >= 0);
            r = eng->RegisterObjectBehaviour("Rect", asBEHAVE_CONSTRUCT, "void f(Vec2&in,float,float)", asFUNCTION(create_rect_tlwh), asCALL_CDECL_OBJLAST); assert(r >= 0);
            r = eng->RegisterObjectBehaviour("Rect", asBEHAVE_CONSTRUCT, "void f(Vec2&in,Vec2&in)", asFUNCTION(create_rect_tlbr), asCALL_CDECL_OBJLAST); assert(r >= 0);
            r = eng->RegisterObjectBehaviour("Rect", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destruct_rect), asCALL_CDECL_OBJLAST); assert(r >= 0);

            r = eng->RegisterObjectMethod("Rect", "Rect& opAssign(Rect&in)", asMETHOD(Rect, operator=), asCALL_THISCALL); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "bool opEquals(Rect&in)", asMETHOD(Rect, operator==), asCALL_THISCALL); assert(r >= 0);

            r = eng->RegisterObjectProperty("Rect", "float Top", asOFFSET(Rect, Top)); assert(r >= 0);
            r = eng->RegisterObjectProperty("Rect", "float Left", asOFFSET(Rect, Left)); assert(r >= 0);
            r = eng->RegisterObjectProperty("Rect", "float Width", asOFFSET(Rect, Width)); assert(r >= 0);
            r = eng->RegisterObjectProperty("Rect", "float Height", asOFFSET(Rect, Height)); assert(r >= 0);

#ifdef AS_SUPPORT_VALRET
            r = eng->RegisterObjectMethod("Rect", "Vec2 get_Center()", asMETHOD(Rect, getCenter), asCALL_THISCALL); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "Vec2 get_TopLeft()", asMETHOD(Rect, getTopLeft), asCALL_THISCALL); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "Vec2 get_BottomRight()", asMETHOD(Rect, getBottomRight), asCALL_THISCALL); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "Vec2 get_Size()", asMETHOD(Rect, getSize), asCALL_THISCALL); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "Vec2 Constrain(Vec2&in)", asMETHOD(Rect, constrain), asCALL_THISCALL); assert(r >= 0);
#else
            r = eng->RegisterObjectMethod("Rect", "Vec2 get_Center()", asFUNCTION(getCenter_rect), asCALL_GENERIC); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "Vec2 get_TopLeft()", asFUNCTION(topLeft_rect), asCALL_GENERIC); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "Vec2 get_BottomRight()", asFUNCTION(bottomRight_rect), asCALL_GENERIC); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "Vec2 get_Size()", asFUNCTION(getSize_rect), asCALL_GENERIC); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "Vec2 Constrain(Vec2&in)", asFUNCTION(constrain_rect), asCALL_GENERIC); assert(r >= 0);
#endif

            r = eng->RegisterObjectMethod("Rect", "void set_TopLeft(Vec2&in)", asFUNCTION(rect_setTopLeft), asCALL_CDECL_OBJLAST); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "void set_BottomRight(Vec2&in)", asFUNCTION(rect_setBottomRight), asCALL_CDECL_OBJLAST); assert(r >= 0);

            r = eng->RegisterObjectMethod("Rect", "bool Contains(Vec2&in)", asMETHOD(Rect, contains), asCALL_THISCALL); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "bool Intersects(Rect&in)", asMETHODPR(Rect, intersects, (const Rect&), bool), asCALL_THISCALL); assert(r >= 0);
            r = eng->RegisterObjectMethod("Rect", "bool Intersects(Rect&in, Rect&out)", asMETHODPR(Rect, intersects, (const Rect&, Rect&), bool), asCALL_THISCALL); assert(r >= 0);
        }, -500);

        return true;
    }
}

bool Script::ScriptExtensions::Rect = Reg();
