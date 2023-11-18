# -*- coding: utf-8 -*-
def t000001000_1():
    """State 0,1"""
    SetEventFlag(82001, 1)
    SetEventFlag(65610, 1)
    SetEventFlag(65640, 1)
    SetEventFlag(65660, 1)
    SetEventFlag(65680, 1)
    SetEventFlag(65720, 1)
    SetEventFlag(65730, 1)
    SetEventFlag(65740, 1)
    SetEventFlag(65750, 1)
    SetEventFlag(65760, 1)
    SetEventFlag(65770, 1)
    SetEventFlag(65780, 1)
    SetEventFlag(65790, 1)
    SetEventFlag(65800, 1)
    SetEventFlag(65810, 1)
    GiveSpEffectToPlayer(330)
    SetEventFlag(62010, 1)
    SetEventFlag(62011, 1)
    SetEventFlag(62012, 1)
    SetEventFlag(62020, 1)
    SetEventFlag(62021, 1)
    SetEventFlag(62022, 1)
    SetEventFlag(62030, 1)
    SetEventFlag(62031, 1)
    SetEventFlag(62032, 1)
    SetEventFlag(62040, 1)
    SetEventFlag(62041, 1)
    SetEventFlag(62050, 1)
    SetEventFlag(62051, 1)
    SetEventFlag(62052, 1)
    SetEventFlag(62060, 1)
    SetEventFlag(62061, 1)
    SetEventFlag(62063, 1)
    SetEventFlag(62062, 1)
    SetEventFlag(62064, 1)
    t000001000_x23()
    Quit()

def t000001000_x0(action2=_):
    """State 0,1"""
    OpenGenericDialog(8, action2, 3, 4, 2)
    assert not CheckSpecificPersonGenericDialogIsOpen(0)
    """State 2"""
    if GetGenericDialogButtonResult() == 1:
        """State 3"""
        return 0
    else:
        """State 4"""
        return 1

def t000001000_x1():
    """State 0,1"""
    if not CheckSpecificPersonTalkHasEnded(0):
        """State 7"""
        ClearTalkProgressData()
        StopEventAnimWithoutForcingConversationEnd(0)
        """State 6"""
        ReportConversationEndToHavokBehavior()
    else:
        pass
    """State 2"""
    if CheckSpecificPersonGenericDialogIsOpen(0) == 1:
        """State 3"""
        ForceCloseGenericDialog()
    else:
        pass
    """State 4"""
    if CheckSpecificPersonMenuIsOpen(-1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0):
        """State 5"""
        ForceCloseMenu()
    else:
        pass
    """State 8"""
    return 0

def t000001000_x2():
    """State 0,1"""
    ClearTalkProgressData()
    StopEventAnimWithoutForcingConversationEnd(0)
    ForceCloseGenericDialog()
    ForceCloseMenu()
    ReportConversationEndToHavokBehavior()
    """State 2"""
    return 0

def t000001000_x3(actionbutton1=_, flag3=6001, flag4=6000):
    """State 0"""
    while True:
        """State 1"""
        assert not GetOneLineHelpStatus() and not IsClientPlayer() and not IsPlayerDead() and not IsCharacterDisabled()
        """State 2"""
        assert CompareBonfireState(1)
        """State 4"""
        if GetEventFlag(flag4) == 1:
            """State 5"""
            assert GetEventFlag(flag3) == 1 and not GetEventFlag(flag4)
            """State 6"""
            assert GetCurrentStateElapsedTime() > 1
        elif GetEventFlag(flag3) == 1 and not GetEventFlag(flag4):
            pass
        """State 3"""
        if CompareBonfireState(0):
            pass
        elif CheckActionButtonArea(actionbutton1):
            break
        elif (not (not GetOneLineHelpStatus() and not IsClientPlayer() and not IsPlayerDead() and not
              IsCharacterDisabled())):
            pass
        elif not GetEventFlag(flag3) or GetEventFlag(flag4) == 1:
            pass
    """State 7"""
    return 0

def t000001000_x4(action1=_):
    """State 0,1"""
    OpenGenericDialog(7, action1, 1, 0, 1)
    assert not CheckSpecificPersonGenericDialogIsOpen(0)
    """State 2"""
    return 0

def t000001000_x5(actionbutton5=6100, flag1=6001, flag2=6000):
    """State 0"""
    while True:
        """State 1"""
        assert not GetOneLineHelpStatus() and not IsClientPlayer() and not IsPlayerDead() and not IsCharacterDisabled()
        """State 3"""
        assert GetEventFlag(flag1) == 1 and not GetEventFlag(flag2)
        """State 2"""
        # actionbutton:6100:"Touch grace"
        if CheckActionButtonArea(actionbutton5):
            break
        elif (not (not GetOneLineHelpStatus() and not IsClientPlayer() and not IsPlayerDead() and not
              IsCharacterDisabled())):
            pass
        elif not GetEventFlag(flag1) or GetEventFlag(flag2) == 1:
            pass
    """State 4"""
    return 0

def t000001000_x6(goods3=1000, goods4=10020):
    """State 0,14"""
    if GetEventFlag(720080) == 1:
        """State 15"""
        pass
    else:
        """State 16,17"""
        SetEventFlag(720080, 1)
    """State 1"""
    if GetTotalBonfireLevel() <= 13:
        """State 2,13,26"""
        # goods:1000:Flask of Crimson Tears, goods:1001:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1003:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1005:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1007:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1009:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1011:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1013:Flask of Crimson Tears +6, goods:1014:Flask of Crimson Tears +7, goods:1015:Flask of Crimson Tears +7, goods:1016:Flask of Crimson Tears +8, goods:1017:Flask of Crimson Tears +8, goods:1018:Flask of Crimson Tears +9, goods:1019:Flask of Crimson Tears +9, goods:1020:Flask of Crimson Tears +10, goods:1021:Flask of Crimson Tears +10, goods:1022:Flask of Crimson Tears +11, goods:1023:Flask of Crimson Tears +11, goods:1024:Flask of Crimson Tears +12, goods:1025:Flask of Crimson Tears +12
        call = t000001000_x18(goods3=goods3, goods6=0, z25=1)
        if call.Get() == 0:
            """State 12,25"""
            # action:13040150:"No Flask of Crimson Tears in inventory"
            assert t000001000_x4(action1=13040150)
        elif call.Done():
            """State 11,19"""
            SetWorkValue(1, 1)
            """State 20"""
            call = t000001000_x0(action2=20011000 + GetWorkValue(1))
            if call.Get() == 0:
                """State 6,10"""
                # goods:10020:Sacred Tear
                if ComparePlayerInventoryNumber(3, goods4, 4, GetWorkValue(1), 0) == 1:
                    """State 4,8"""
                    BonfireActivation(GetTotalBonfireLevel() + 1)
                    """State 9"""
                    # goods:10020:Sacred Tear
                    PlayerEquipmentQuantityChange(3, goods4, GetWorkValue(1) * -1)
                    """State 27"""
                    assert t000001000_x20(goods3=goods3)
                    """State 24"""
                    assert t000001000_x17(goods5=goods3)
                    """State 22"""
                    # action:13040020:"Increased the amount of HP/FP replenished by your flasks"
                    assert t000001000_x4(action1=13040020)
                    """State 18"""
                    SetWorkValue(1, 0)
                else:
                    """State 5,28"""
                    # action:20011000:"No Sacred Tear in inventory"
                    assert t000001000_x4(action1=20011000)
            elif call.Done():
                """State 7"""
                pass
    else:
        """State 3,21"""
        # action:13040000:"The amount of HP/FP your flasks replenish cannot be increased any further"
        assert t000001000_x4(action1=13040000)
    """State 29"""
    return 0

def t000001000_x7(goods1=1000, goods2=10010):
    """State 0,15"""
    if GetEventFlag(720070) == 1:
        """State 16"""
        pass
    else:
        """State 17,18"""
        SetEventFlag(720070, 1)
    """State 1"""
    if GetEstusAllocation(0) + GetEstusAllocation(1) < 13:
        """State 2,13,26"""
        # goods:1000:Flask of Crimson Tears, goods:1001:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1003:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1005:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1007:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1009:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1011:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1013:Flask of Crimson Tears +6, goods:1014:Flask of Crimson Tears +7, goods:1015:Flask of Crimson Tears +7, goods:1016:Flask of Crimson Tears +8, goods:1017:Flask of Crimson Tears +8, goods:1018:Flask of Crimson Tears +9, goods:1019:Flask of Crimson Tears +9, goods:1020:Flask of Crimson Tears +10, goods:1021:Flask of Crimson Tears +10, goods:1022:Flask of Crimson Tears +11, goods:1023:Flask of Crimson Tears +11, goods:1024:Flask of Crimson Tears +12, goods:1025:Flask of Crimson Tears +12
        call = t000001000_x18(goods3=goods1, goods6=0, z25=1)
        if call.Get() == 0:
            """State 12,25"""
            # action:13040150:"No Flask of Crimson Tears in inventory"
            assert t000001000_x4(action1=13040150)
        elif call.Done():
            """State 11,19"""
            SetWorkValue(1, GetEstusAllocation(0) + GetEstusAllocation(1) + -4)
            """State 28"""
            assert (t000001000_x22(z12=1, z13=1, z14=1, z15=1, z16=1, z17=1, z18=1, z19=1, z20=1, z21=1,
                    z22=1))
            """State 21"""
            call = t000001000_x0(action2=20011010 + GetWorkValue(1))
            if call.Get() == 0:
                """State 6,14"""
                # goods:10010:Golden Seed
                if ComparePlayerInventoryNumber(3, goods2, 4, GetWorkValue(1), 0) == 1:
                    """State 4,8"""
                    # goods:10010:Golden Seed
                    PlayerEquipmentQuantityChange(3, goods2, GetWorkValue(1) * -1)
                    """State 9"""
                    EstusAllocationUpdate(GetEstusAllocation(0) + 1, 0)
                    """State 27"""
                    assert t000001000_x17(goods5=goods1)
                    """State 22"""
                    # action:13040140:"Added a charge to Flask of Crimson Tears"
                    assert t000001000_x4(action1=13040140)
                    """State 20"""
                    SetWorkValue(1, 0)
                    """State 10"""
                else:
                    """State 5,23"""
                    # action:20011010:"Not enough Golden Seeds"
                    assert t000001000_x4(action1=20011010)
            elif call.Done():
                """State 7"""
                pass
    else:
        """State 3,24"""
        # action:13040120:"Flask charges already at maximum"
        assert t000001000_x4(action1=13040120)
    """State 29"""
    return 0

def t000001000_x8(goods7=1000):
    """State 0,1"""
    # goods:1001:Flask of Crimson Tears, goods:1000:Flask of Crimson Tears
    if DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 0 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 0 * 2) == 1:
        """State 2"""
        BonfireActivation(1)
        """State 13"""
        Label('L0')
        """State 18"""
        assert t000001000_x9(goods7=goods7)
    # goods:1003:Flask of Crimson Tears +1, goods:1002:Flask of Crimson Tears +1
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 1 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 1 * 2) == 1:
        """State 3"""
        BonfireActivation(2)
        Goto('L0')
    # goods:1005:Flask of Crimson Tears +2, goods:1004:Flask of Crimson Tears +2
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 2 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 2 * 2) == 1:
        """State 4"""
        BonfireActivation(3)
        Goto('L0')
    # goods:1007:Flask of Crimson Tears +3, goods:1006:Flask of Crimson Tears +3
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 3 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 3 * 2) == 1:
        """State 5"""
        BonfireActivation(4)
        Goto('L0')
    # goods:1009:Flask of Crimson Tears +4, goods:1008:Flask of Crimson Tears +4
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 4 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 4 * 2) == 1:
        """State 6"""
        BonfireActivation(5)
        Goto('L0')
    # goods:1011:Flask of Crimson Tears +5, goods:1010:Flask of Crimson Tears +5
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 5 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 5 * 2) == 1:
        """State 7"""
        BonfireActivation(6)
        Goto('L0')
    # goods:1013:Flask of Crimson Tears +6, goods:1012:Flask of Crimson Tears +6
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 6 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 6 * 2) == 1:
        """State 8"""
        BonfireActivation(7)
        Goto('L0')
    # goods:1015:Flask of Crimson Tears +7, goods:1014:Flask of Crimson Tears +7
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 7 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 7 * 2) == 1:
        """State 9"""
        BonfireActivation(8)
        Goto('L0')
    # goods:1017:Flask of Crimson Tears +8, goods:1016:Flask of Crimson Tears +8
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 8 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 8 * 2) == 1:
        """State 10"""
        BonfireActivation(9)
        Goto('L0')
    # goods:1019:Flask of Crimson Tears +9, goods:1018:Flask of Crimson Tears +9
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 9 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 9 * 2) == 1:
        """State 11"""
        BonfireActivation(10)
        Goto('L0')
    # goods:1021:Flask of Crimson Tears +10, goods:1020:Flask of Crimson Tears +10
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 10 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 10 * 2) == 1:
        """State 12"""
        BonfireActivation(11)
        Goto('L0')
    elif True:
        """State 15"""
        pass
    # goods:1023:Flask of Crimson Tears +11, goods:1022:Flask of Crimson Tears +11
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 11 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 11 * 2) == 1:
        """State 16"""
        BonfireActivation(12)
        Goto('L0')
    # goods:1025:Flask of Crimson Tears +12, goods:1024:Flask of Crimson Tears +12
    elif DoesPlayerHaveItem(3, goods7 + 1 + 0 * 50 + 12 * 2) == 1 or DoesPlayerHaveItem(3, goods7 + 0 + 0 * 50 + 12 * 2) == 1:
        """State 17"""
        BonfireActivation(13)
        Goto('L0')
    """State 14,19"""
    return 0

def t000001000_x9(goods7=1000):
    """State 0,15"""
    # goods:1000:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1014:Flask of Crimson Tears +7
    call = t000001000_x12(goods7=goods7, goods8=0, goods9=0)
    if call.Get() == 1:
        """State 1,11"""
        # goods:1000:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6
        assert t000001000_x10(goods7=goods7, goods10=0, goods11=0)
    elif call.Done():
        """State 16"""
        # goods:1001:Flask of Crimson Tears, goods:1003:Flask of Crimson Tears +1, goods:1005:Flask of Crimson Tears +2, goods:1007:Flask of Crimson Tears +3, goods:1009:Flask of Crimson Tears +4, goods:1011:Flask of Crimson Tears +5, goods:1013:Flask of Crimson Tears +6, goods:1015:Flask of Crimson Tears +7
        call = t000001000_x12(goods7=goods7, goods8=0, goods9=1)
        if call.Get() == 1:
            """State 8,10"""
            # goods:1001:Flask of Crimson Tears, goods:1003:Flask of Crimson Tears +1, goods:1005:Flask of Crimson Tears +2, goods:1007:Flask of Crimson Tears +3, goods:1009:Flask of Crimson Tears +4, goods:1011:Flask of Crimson Tears +5, goods:1013:Flask of Crimson Tears +6
            assert t000001000_x10(goods7=goods7, goods10=0, goods11=1)
        elif call.Done():
            """State 2"""
            pass
    """State 5,17"""
    # goods:1050:Flask of Cerulean Tears, goods:1052:Flask of Cerulean Tears +1, goods:1054:Flask of Cerulean Tears +2, goods:1056:Flask of Cerulean Tears +3, goods:1058:Flask of Cerulean Tears +4, goods:1060:Flask of Cerulean Tears +5, goods:1062:Flask of Cerulean Tears +6, goods:1064:Flask of Cerulean Tears +7
    call = t000001000_x12(goods7=goods7, goods8=1, goods9=0)
    if call.Get() == 1:
        """State 3,13"""
        # goods:1050:Flask of Cerulean Tears, goods:1052:Flask of Cerulean Tears +1, goods:1054:Flask of Cerulean Tears +2, goods:1056:Flask of Cerulean Tears +3, goods:1058:Flask of Cerulean Tears +4, goods:1060:Flask of Cerulean Tears +5, goods:1062:Flask of Cerulean Tears +6
        assert t000001000_x10(goods7=goods7, goods10=1, goods11=0)
    elif call.Done():
        """State 18"""
        # goods:1051:Flask of Cerulean Tears, goods:1053:Flask of Cerulean Tears +1, goods:1055:Flask of Cerulean Tears +2, goods:1057:Flask of Cerulean Tears +3, goods:1059:Flask of Cerulean Tears +4, goods:1061:Flask of Cerulean Tears +5, goods:1063:Flask of Cerulean Tears +6, goods:1065:Flask of Cerulean Tears +7
        call = t000001000_x12(goods7=goods7, goods8=1, goods9=1)
        if call.Get() == 1:
            """State 9,12"""
            # goods:1051:Flask of Cerulean Tears, goods:1053:Flask of Cerulean Tears +1, goods:1055:Flask of Cerulean Tears +2, goods:1057:Flask of Cerulean Tears +3, goods:1059:Flask of Cerulean Tears +4, goods:1061:Flask of Cerulean Tears +5, goods:1063:Flask of Cerulean Tears +6
            assert t000001000_x10(goods7=goods7, goods10=1, goods11=1)
        elif call.Done():
            """State 4"""
            pass
    """State 6,14"""
    assert t000001000_x11()
    """State 7,19"""
    return 0

def t000001000_x10(goods7=1000, goods10=_, goods11=_):
    """State 0,1"""
    if DoesPlayerHaveItem(3, goods7 + goods11 + goods10 * 50 + 0 * 2) == 1:
        """State 3"""
        ReplaceTool(goods7 + goods10 * 50 + 0 * 2 + goods11, goods7 + goods10 * 50 + goods11 + 2 * (GetTotalBonfireLevel() - 1),
                    1)
    elif DoesPlayerHaveItem(3, goods7 + goods11 + goods10 * 50 + 1 * 2) == 1:
        """State 4"""
        ReplaceTool(goods7 + goods10 * 50 + 1 * 2 + goods11, goods7 + goods10 * 50 + goods11 + 2 * (GetTotalBonfireLevel() - 1),
                    1)
    elif DoesPlayerHaveItem(3, goods7 + goods11 + goods10 * 50 + 2 * 2) == 1:
        """State 5"""
        ReplaceTool(goods7 + goods10 * 50 + 2 * 2 + goods11, goods7 + goods10 * 50 + goods11 + 2 * (GetTotalBonfireLevel() - 1),
                    1)
    elif DoesPlayerHaveItem(3, goods7 + goods11 + goods10 * 50 + 3 * 2) == 1:
        """State 6"""
        ReplaceTool(goods7 + goods10 * 50 + 3 * 2 + goods11, goods7 + goods10 * 50 + goods11 + 2 * (GetTotalBonfireLevel() - 1),
                    1)
    elif DoesPlayerHaveItem(3, goods7 + goods11 + goods10 * 50 + 4 * 2) == 1:
        """State 7"""
        ReplaceTool(goods7 + goods10 * 50 + 4 * 2 + goods11, goods7 + goods10 * 50 + goods11 + 2 * (GetTotalBonfireLevel() - 1),
                    1)
    elif DoesPlayerHaveItem(3, goods7 + goods11 + goods10 * 50 + 5 * 2) == 1:
        """State 8"""
        ReplaceTool(goods7 + goods10 * 50 + 5 * 2 + goods11, goods7 + goods10 * 50 + goods11 + 2 * (GetTotalBonfireLevel() - 1),
                    1)
    elif DoesPlayerHaveItem(3, goods7 + goods11 + goods10 * 50 + 6 * 2) == 1:
        """State 9"""
        ReplaceTool(goods7 + goods10 * 50 + 6 * 2 + goods11, goods7 + goods10 * 50 + goods11 + 2 * (GetTotalBonfireLevel() - 1),
                    1)
    else:
        """State 2"""
        pass
    """State 13"""
    return 0

def t000001000_x11():
    """State 0,1,13,14"""
    return 0

def t000001000_x12(goods7=1000, goods8=_, goods9=_):
    """State 0,1"""
    if (not DoesPlayerHaveItem(3, goods7 + goods9 + goods8 * 50 + 0 * 2) and not DoesPlayerHaveItem(3,
        goods7 + goods9 + goods8 * 50 + 1 * 2) and not DoesPlayerHaveItem(3, goods7 + goods9 + goods8
        * 50 + 2 * 2) and not DoesPlayerHaveItem(3, goods7 + goods9 + goods8 * 50 + 3 * 2) and not DoesPlayerHaveItem(3,
        goods7 + goods9 + goods8 * 50 + 4 * 2) and not DoesPlayerHaveItem(3, goods7 + goods9 + goods8
        * 50 + 5 * 2) and not DoesPlayerHaveItem(3, goods7 + goods9 + goods8 * 50 + 6 * 2) and not DoesPlayerHaveItem(3,
        goods7 + goods9 + goods8 * 50 + 7 * 2)):
        """State 2,3"""
        return 0
    else:
        """State 4"""
        return 1

def t000001000_x13():
    """State 0,1"""
    if DoesPlayerHaveSpEffect(8990) == 1:
        """State 2"""
        GiveSpEffectToPlayer(8998)
        """State 3"""
        SetEventFlag(100210, 0)
        SetEventFlag(100200, 0)
    else:
        pass
    """State 4"""
    return 0

def t000001000_x14(text2=_, z26=_, mode3=1):
    """State 0,5"""
    assert t000001000_x2() and CheckSpecificPersonTalkHasEnded(0) == 1
    """State 1"""
    TalkToPlayer(text2, -1, -1, 0)
    assert CheckSpecificPersonTalkHasEnded(0) == 1
    """State 4"""
    if not mode3:
        pass
    else:
        """State 3"""
        ReportConversationEndToHavokBehavior()
    """State 2"""
    SetEventFlag(z26, 1)
    """State 6"""
    return 0

def t000001000_x15(text1=_, mode2=1):
    """State 0,4"""
    assert t000001000_x2() and CheckSpecificPersonTalkHasEnded(0) == 1
    """State 1"""
    TalkToPlayer(text1, -1, -1, 0)
    assert CheckSpecificPersonTalkHasEnded(0) == 1
    """State 3"""
    if not mode2:
        pass
    else:
        """State 2"""
        ReportConversationEndToHavokBehavior()
    """State 5"""
    return 0

def t000001000_x16(goods5=1000):
    """State 0,4,10"""
    # goods:1000:Flask of Crimson Tears, goods:1001:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1003:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1005:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1007:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1009:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1011:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1013:Flask of Crimson Tears +6, goods:1014:Flask of Crimson Tears +7, goods:1015:Flask of Crimson Tears +7, goods:1016:Flask of Crimson Tears +8, goods:1017:Flask of Crimson Tears +8, goods:1018:Flask of Crimson Tears +9, goods:1019:Flask of Crimson Tears +9, goods:1020:Flask of Crimson Tears +10, goods:1021:Flask of Crimson Tears +10, goods:1022:Flask of Crimson Tears +11, goods:1023:Flask of Crimson Tears +11, goods:1024:Flask of Crimson Tears +12, goods:1025:Flask of Crimson Tears +12
    call = t000001000_x18(goods3=goods5, goods6=0, z25=1)
    if call.Get() == 0:
        """State 2,8"""
        # action:13040150:"No Flask of Crimson Tears in inventory"
        assert t000001000_x4(action1=13040150)
    elif call.Done():
        """State 1,7,12"""
        # goods:1050:Flask of Cerulean Tears, goods:1051:Flask of Cerulean Tears, goods:1052:Flask of Cerulean Tears +1, goods:1053:Flask of Cerulean Tears +1, goods:1054:Flask of Cerulean Tears +2, goods:1055:Flask of Cerulean Tears +2, goods:1056:Flask of Cerulean Tears +3, goods:1057:Flask of Cerulean Tears +3, goods:1058:Flask of Cerulean Tears +4, goods:1059:Flask of Cerulean Tears +4, goods:1060:Flask of Cerulean Tears +5, goods:1061:Flask of Cerulean Tears +5, goods:1062:Flask of Cerulean Tears +6, goods:1063:Flask of Cerulean Tears +6, goods:1064:Flask of Cerulean Tears +7, goods:1065:Flask of Cerulean Tears +7, goods:1066:Flask of Cerulean Tears +8, goods:1067:Flask of Cerulean Tears +8, goods:1068:Flask of Cerulean Tears +9, goods:1069:Flask of Cerulean Tears +9, goods:1070:Flask of Cerulean Tears +10, goods:1071:Flask of Cerulean Tears +10, goods:1072:Flask of Cerulean Tears +11, goods:1073:Flask of Cerulean Tears +11, goods:1074:Flask of Cerulean Tears +12, goods:1075:Flask of Cerulean Tears +12
        call = t000001000_x18(goods3=goods5, goods6=1, z25=1)
        if call.Get() == 0:
            """State 6,11"""
            # action:13040160:"No Flask of Cerulean Tears in inventory"
            assert t000001000_x4(action1=13040160)
        elif call.Done():
            """State 5,3"""
            OpenEstusAllotMenu()
            assert not (CheckSpecificPersonMenuIsOpen(14, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
            """State 9"""
            assert t000001000_x17(goods5=goods5)
    """State 13"""
    return 0

def t000001000_x17(goods5=1000):
    """State 0,3"""
    # goods:1000:Flask of Crimson Tears, goods:1001:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1003:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1005:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1007:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1009:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1011:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1013:Flask of Crimson Tears +6, goods:1014:Flask of Crimson Tears +7, goods:1015:Flask of Crimson Tears +7, goods:1016:Flask of Crimson Tears +8, goods:1017:Flask of Crimson Tears +8, goods:1018:Flask of Crimson Tears +9, goods:1019:Flask of Crimson Tears +9, goods:1020:Flask of Crimson Tears +10, goods:1021:Flask of Crimson Tears +10, goods:1022:Flask of Crimson Tears +11, goods:1023:Flask of Crimson Tears +11, goods:1024:Flask of Crimson Tears +12, goods:1025:Flask of Crimson Tears +12
    call = t000001000_x18(goods3=goods5, goods6=0, z25=1)
    if call.Get() == 1:
        """State 4"""
        assert t000001000_x19(goods5=goods5, mode1=0, z24=GetWorkValue(1))
        """State 5"""
        assert t000001000_x19(goods5=goods5, mode1=1, z24=GetWorkValue(1))
    elif call.Done():
        """State 1"""
        pass
    """State 2"""
    SetWorkValue(1, 0)
    """State 6"""
    return 0

def t000001000_x18(goods3=1000, goods6=_, z25=1):
    """State 0,13"""
    SetWorkValue(z25, 0)
    if (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 0 * 2) == 1 or DoesPlayerHaveItem(3, goods3
        + 1 + goods6 * 50 + 0 * 2) == 1):
        """State 1"""
        SetWorkValue(z25, 0)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 1 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 1 * 2) == 1):
        """State 2"""
        SetWorkValue(z25, 1)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 2 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 2 * 2) == 1):
        """State 3"""
        SetWorkValue(z25, 2)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 3 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 3 * 2) == 1):
        """State 4"""
        SetWorkValue(z25, 3)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 4 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 4 * 2) == 1):
        """State 5"""
        SetWorkValue(z25, 4)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 5 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 5 * 2) == 1):
        """State 6"""
        SetWorkValue(z25, 5)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 6 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 6 * 2) == 1):
        """State 7"""
        SetWorkValue(z25, 6)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 7 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 7 * 2) == 1):
        """State 8"""
        SetWorkValue(z25, 7)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 8 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 8 * 2) == 1):
        """State 9"""
        SetWorkValue(z25, 8)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 9 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 9 * 2) == 1):
        """State 10"""
        SetWorkValue(z25, 9)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 10 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 10 * 2) == 1):
        """State 11"""
        SetWorkValue(z25, 10)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 11 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 11 * 2) == 1):
        """State 14"""
        SetWorkValue(z25, 11)
    elif (DoesPlayerHaveItem(3, goods3 + 0 + goods6 * 50 + 12 * 2) == 1 or DoesPlayerHaveItem(3, goods3
          + 1 + goods6 * 50 + 12 * 2) == 1):
        """State 15"""
        SetWorkValue(z25, 12)
    else:
        """State 12"""
        SetWorkValue(z25, 13)
        """State 16"""
        return 0
    """State 17"""
    return 1

def t000001000_x19(goods5=1000, mode1=_, z24=_):
    """State 0,6"""
    if not GetEstusAllocation(mode1) < 0:
        """State 7,12"""
        if (DoesPlayerHaveItem(3, goods5 + 0 + mode1 * 50 + z24 * 2) == 1 or DoesPlayerHaveItem(3, goods5
            + 1 + mode1 * 50 + z24 * 2) == 1):
            """State 13,1"""
            if DoesPlayerHaveItem(3, goods5 + 0 + mode1 * 50 + z24 * 2) == 1:
                """State 2,4"""
                ReplaceTool(1000 + mode1 * 50 + z24 * 2 + 0, 1000 + mode1 * 50 + z24 * 2 + 1, 1)
            else:
                """State 3"""
                pass
            while True:
                """State 5"""
                if (0 == ComparePlayerInventoryNumber(3, 1000 + 1 + mode1 * 50 + z24 * 2, 4, GetEstusAllocation(mode1),
                    0)):
                    """State 9,11"""
                    PlayerEquipmentQuantityChange(3, 1000 + 1 + mode1 * 50 + z24 * 2, 1)
                else:
                    """State 10"""
                    break
        else:
            """State 14"""
            pass
    else:
        """State 8"""
        pass
    """State 15"""
    return 0

def t000001000_x20(goods3=1000):
    """State 0,3"""
    # goods:1000:Flask of Crimson Tears, goods:1001:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1003:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1005:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1007:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1009:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1011:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1013:Flask of Crimson Tears +6, goods:1014:Flask of Crimson Tears +7, goods:1015:Flask of Crimson Tears +7, goods:1016:Flask of Crimson Tears +8, goods:1017:Flask of Crimson Tears +8, goods:1018:Flask of Crimson Tears +9, goods:1019:Flask of Crimson Tears +9, goods:1020:Flask of Crimson Tears +10, goods:1021:Flask of Crimson Tears +10, goods:1022:Flask of Crimson Tears +11, goods:1023:Flask of Crimson Tears +11, goods:1024:Flask of Crimson Tears +12, goods:1025:Flask of Crimson Tears +12
    call = t000001000_x18(goods3=goods3, goods6=0, z25=1)
    if call.Get() == 1:
        """State 4"""
        assert t000001000_x21(goods3=goods3, z23=GetWorkValue(1))
        """State 2"""
        SetWorkValue(1, 0)
    elif call.Done():
        """State 1"""
        pass
    """State 5"""
    return 0

def t000001000_x21(goods3=1000, z23=_):
    """State 0,1"""
    ReplaceToolIf(DoesPlayerHaveItem(3, 1000 + 0 + 0 * 50 + z23 * 2) == 1, goods3 + 0 * 50 + z23 * 2 + 0,
                  goods3 + 0 * 50 + 0 + 2 * (GetTotalBonfireLevel() - 1), 1)
    """State 2"""
    ReplaceToolIf(DoesPlayerHaveItem(3, 1000 + 1 + 0 * 50 + z23 * 2) == 1, goods3 + 0 * 50 + z23 * 2 + 1,
                  goods3 + 0 * 50 + 1 + 2 * (GetTotalBonfireLevel() - 1), 1)
    """State 3"""
    ReplaceToolIf(DoesPlayerHaveItem(3, 1000 + 0 + 1 * 50 + z23 * 2) == 1, goods3 + 1 * 50 + z23 * 2 + 0,
                  goods3 + 1 * 50 + 0 + 2 * (GetTotalBonfireLevel() - 1), 1)
    """State 4"""
    ReplaceToolIf(DoesPlayerHaveItem(3, 1000 + 1 + 1 * 50 + z23 * 2) == 1, goods3 + 1 * 50 + z23 * 2 + 1,
                  goods3 + 1 * 50 + 1 + 2 * (GetTotalBonfireLevel() - 1), 1)
    """State 5"""
    return 0

def t000001000_x22(z12=1, z13=1, z14=1, z15=1, z16=1, z17=1, z18=1, z19=1, z20=1, z21=1, z22=1):
    """State 0"""
    if not GetWorkValue(z22):
        """State 1"""
        SetWorkValue(z22, z12)
    elif GetWorkValue(z22) == 1:
        """State 2"""
        SetWorkValue(z22, z13)
    elif GetWorkValue(z22) == 2:
        """State 3"""
        SetWorkValue(z22, z14)
    elif GetWorkValue(z22) == 3:
        """State 4"""
        SetWorkValue(z22, z15)
    elif GetWorkValue(z22) == 4:
        """State 5"""
        SetWorkValue(z22, z16)
    elif GetWorkValue(z22) == 5:
        """State 6"""
        SetWorkValue(z22, z17)
    elif GetWorkValue(z22) == 6:
        """State 7"""
        SetWorkValue(z22, z18)
    elif GetWorkValue(z22) == 7:
        """State 8"""
        SetWorkValue(z22, z19)
    elif GetWorkValue(z22) == 8:
        """State 9"""
        SetWorkValue(z22, z20)
    elif GetWorkValue(z22) == 9:
        """State 10"""
        SetWorkValue(z22, z21)
    else:
        """State 11"""
        SetWorkValue(z22, 5)
    """State 12"""
    return 0

def t000001000_x23():
    """State 0"""
    if not GetEventFlag(9000):
        """State 3,1"""
        SetEventFlag(4652, 0)
        SetEventFlag(4653, 0)
        SetEventFlag(4654, 0)
        SetEventFlag(4655, 0)
        SetEventFlag(4656, 0)
        SetEventFlag(4657, 0)
        SetEventFlag(4651, 0)
    else:
        """State 2"""
        pass
    while True:
        """State 4"""
        if IsMultiplayerInProgress() == 1:
            """State 6"""
            call = t000001000_x26()
            assert not IsMultiplayerInProgress()
        elif GetEventFlag(1042369415) == 1:
            """State 7"""
            call = t000001000_x64()
            assert not GetEventFlag(1042369415)
        else:
            """State 5"""
            def WhilePaused():
                GiveSpEffectToPlayerIf(GetEventFlag(9000) == 1 and not GetWorkValue(0), 9607)
                GiveSpEffectToPlayerIf(GetEventFlag(9000) == 1 and GetWorkValue(0) == 10, 9608)
                GiveSpEffectToPlayerIf(GetEventFlag(9000) == 1 and GetDistanceToPlayer() < 1.05, 9609)
                GiveSpEffectToPlayerIf(GetEventFlag(4698) == 1 and GetEventFlag(9001) == 1, 9606)
            assert t000001000_x24()

def t000001000_x24():
    """State 0,1"""
    if CompareBonfireLevel(5, 0) == 1 or GetEventFlag(11102790) == 1:
        """State 2"""
        Label('L0')
        assert GetWhetherChrEventAnimHasEnded(10000) == 1
    else:
        """State 3,30"""
        call = t000001000_x41()
        if call.Done():
            """State 7"""
            TurnCharacterToFaceEntity(-1, 10000, -1, -1)
            assert GetCurrentStateElapsedFrames() > 1 and GetWhetherChrEventAnimHasEnded(10000) == 1
            """State 4"""
            c1_40()
            assert CompareBonfireLevel(5, 0) == 1
            """State 9,10"""
            UpdatePlayerRespawnPoint()
            Goto('L0')
        elif CompareBonfireLevel(5, 0) == 1 or GetEventFlag(11102790) == 1:
            pass
    """State 31"""
    call = t000001000_x41()
    if call.Done():
        """State 5"""
        ClearPlayerDamageInfo()
        """State 11"""
        GiveSpEffectToPlayer(202)
        """State 6"""
        SetTalkTime(1)
        """State 35"""
        assert t000001000_x68()
        """State 23"""
        SetEventFlag(4698, 0)
        SetEventFlagIf(DoesSelfHaveSpEffect(9680) == 1 and not GetEventFlag(953), 4698, 1)
        SetEventFlagIf(GetEventFlagValue(955, 3) > 2 and DoesSelfHaveSpEffect(9681) == 1 and not GetEventFlag(953),
                       4698, 1)
        """State 28"""
        assert t000001000_x33()
        """State 8"""
        TurnCharacterToFaceEntity(-1, 10000, -1, -1)
        assert GetCurrentStateElapsedFrames() > 1 and GetWhetherChrEventAnimHasEnded(10000) == 1
        """State 12"""
        UpdatePlayerRespawnPoint()
        """State 19"""
        StartBonfireAnimLoop(1, 1, 1, 1, GetWorkValue(0), 0.5)
        """State 13"""
        c1_117(0, 0, 0, 0, 0, 0, 0, 0.1, 0, 1.5, 0, 0.75, 0.5)
        """State 18"""
        SetEventFlag(9000, 1)
        SetEventFlag(9020, 1)
        c5_138(not GetEventFlag(11102790), 1001000)
        c5_138(GetEventFlag(11102790) == 1, 1001001)
        if not GetEventFlag(4698):
            """State 34"""
            assert t000001000_x67()
        else:
            """State 33"""
            assert t000001000_x65()
        """State 17"""
        if not GetEventFlag(9001):
            """State 21"""
            pass
        else:
            """State 20"""
            assert not GetEventFlag(9001)
        """State 27"""
        assert t000001000_x31()
        """State 15"""
        if not GetEventFlag(4653):
            """State 22,37"""
            Label('L1')
            assert t000001000_x69()
            """State 29"""
            call = t000001000_x32()
            def WhilePaused():
                GiveSpEffectToPlayerIf(GetEventFlag(4651) == 1, 9620)
            def ExitPause():
                EndBonfireKindleAnimLoop(GetWorkValue(0))
            if call.Done():
                pass
            elif (((GetDistanceToPlayer() > 3 and not GetEventFlag(11102790)) or GetDistanceToPlayer()
                  > 7) and GetCurrentStateElapsedFrames() > 1):
                """State 26"""
                Label('L2')
                assert t000001000_x25()
            elif CompareBonfireState(0) and GetCurrentStateElapsedFrames() > 1:
                Goto('L2')
            elif HasPlayerBeenAttacked() == 1 and GetCurrentStateElapsedFrames() > 1:
                Goto('L2')
            elif IsMultiplayerInProgress() == 1 and GetCurrentStateElapsedFrames() > 1:
                Goto('L2')
            elif GetEventFlag(1042369415) == 1 and GetCurrentStateElapsedFrames() > 1:
                Goto('L2')
            elif (CompareBonfireLevel(0, 0) == 1 and not GetEventFlag(11102790) and GetCurrentStateElapsedFrames()
                  > 1):
                Goto('L2')
        else:
            """State 16,32"""
            call = t000001000_x51()
            if call.Done():
                Goto('L1')
            elif GetEventFlag(1042369415) == 1 and GetCurrentStateElapsedFrames() > 1:
                """State 24"""
                Label('L3')
                def ExitPause():
                    EndBonfireKindleAnimLoop(GetWorkValue(0))
                Goto('L2')
            elif IsMultiplayerInProgress() == 1 and GetCurrentStateElapsedFrames() > 1:
                Goto('L3')
            elif HasPlayerBeenAttacked() == 1 and GetCurrentStateElapsedFrames() > 1:
                Goto('L3')
            elif (((GetDistanceToPlayer() > 3 and not GetEventFlag(11102790)) or GetDistanceToPlayer()
                  > 7) and GetCurrentStateElapsedFrames() > 1):
                Goto('L3')
            elif CompareBonfireState(0) and GetCurrentStateElapsedFrames() > 1:
                Goto('L3')
        """State 36"""
        assert t000001000_x68()
        """State 14"""
        SetEventFlag(9000, 0)
        SetEventFlag(9020, 0)
        c1_138(-1)
        c1_140(0)
        assert GetCurrentStateElapsedFrames() > 1
        """State 25"""
        if not IsMultiplayerInProgress() and not GetEventFlag(1042369415):
            Goto('L0')
        else:
            pass
    elif (IsMultiplayerInProgress() == 1 or GetEventFlag(1042369415) == 1 or (CompareBonfireLevel(0,
          0) == 1 and not GetEventFlag(11102790))):
        pass
    """State 38"""
    return 0

def t000001000_x25():
    """State 0,1"""
    assert t000001000_x1()
    """State 2"""
    return 0

def t000001000_x26():
    """State 0"""
    while True:
        """State 1"""
        call = t000001000_x27()
        assert IsClientPlayer() == 1
        """State 2"""
        call = t000001000_x28()
        assert not IsClientPlayer()

def t000001000_x27():
    """State 0,6"""
    call = t000001000_x1()
    if call.Done() and CompareBonfireLevel(5, 0) == 1:
        pass
    elif call.Done():
        """State 2,7"""
        # actionbutton:6100:"Touch grace"
        call = t000001000_x3(actionbutton1=6100, flag3=6001, flag4=6000)
        if call.Done():
            """State 4"""
            TurnCharacterToFaceEntity(-1, 10000, -1, -1)
            assert GetCurrentStateElapsedFrames() > 1 and GetWhetherChrEventAnimHasEnded(10000) == 1
            """State 3"""
            c1_40()
            """State 5"""
            UpdatePlayerRespawnPoint()
            assert CompareBonfireLevel(5, 0) == 1
        elif CompareBonfireLevel(5, 0) == 1:
            pass
    """State 1"""
    Quit()

def t000001000_x28():
    """State 0,1"""
    assert t000001000_x1()
    """State 2"""
    return 0

def t000001000_x29():
    """State 0,1"""
    if not GetEventFlag(4651):
        """State 3"""
        if GetEventFlag(4698) == 1:
            """State 5,10"""
            assert t000001000_x36(z7=20006, val4=0.5, z8=1, z9=2, z10=60)
        elif GetEventFlag(108) == 1:
            """State 9,13"""
            assert t000001000_x36(z7=20000, val4=3.5, z8=1, z9=2, z10=60)
        else:
            """State 4,6"""
            # eventflag:400001:lot:100010:Rold Medallion
            if not GetEventFlag(400001):
                """State 7,11"""
                assert t000001000_x36(z7=20000, val4=3.5, z8=1, z9=2, z10=60)
            else:
                """State 8,12"""
                assert t000001000_x36(z7=20001, val4=3.5, z8=1, z9=2, z10=60)
    else:
        """State 2"""
        pass
    """State 14"""
    return 0

def t000001000_x30(z11=_):
    """State 0,1"""
    c1_129(z11, 1.4)
    """State 2"""
    SetEventFlag(4651, 0)
    SetEventFlag(4652, 0)
    SetEventFlag(4653, 0)
    SetEventFlag(4654, 0)
    SetEventFlag(4655, 0)
    SetEventFlag(4656, 0)
    SetEventFlag(4657, 0)
    """State 3"""
    return 0

def t000001000_x31():
    """State 0"""
    if GetEventFlag(110) == 1:
        """State 1"""
        pass
    elif not GetEventFlag(953) or GetEventFlag(4698) == 1:
        """State 3"""
        assert t000001000_x60()
    elif not GetEventFlag(4680):
        """State 2"""
        pass
    elif GetEventFlag(108) == 1:
        """State 6"""
        assert t000001000_x63()
    # eventflag:400001:lot:100010:Rold Medallion
    elif GetEventFlag(11009260) == 1 and not GetEventFlag(400001):
        """State 5"""
        assert t000001000_x62()
    else:
        """State 4"""
        assert t000001000_x61()
    """State 7"""
    return 0

def t000001000_x32():
    """State 0,10"""
    assert GetCurrentStateElapsedTime() > 0.1 or not GetEventFlag(4651)
    """State 11"""
    assert not GetEventFlag(9001)
    """State 17"""
    # goods:1001:Flask of Crimson Tears, goods:1000:Flask of Crimson Tears, goods:1003:Flask of Crimson Tears +1, goods:1002:Flask of Crimson Tears +1, goods:1005:Flask of Crimson Tears +2, goods:1004:Flask of Crimson Tears +2, goods:1007:Flask of Crimson Tears +3, goods:1006:Flask of Crimson Tears +3, goods:1009:Flask of Crimson Tears +4, goods:1008:Flask of Crimson Tears +4, goods:1011:Flask of Crimson Tears +5, goods:1010:Flask of Crimson Tears +5, goods:1013:Flask of Crimson Tears +6, goods:1012:Flask of Crimson Tears +6, goods:1015:Flask of Crimson Tears +7, goods:1014:Flask of Crimson Tears +7, goods:1017:Flask of Crimson Tears +8, goods:1016:Flask of Crimson Tears +8, goods:1019:Flask of Crimson Tears +9, goods:1018:Flask of Crimson Tears +9, goods:1021:Flask of Crimson Tears +10, goods:1020:Flask of Crimson Tears +10, goods:1023:Flask of Crimson Tears +11, goods:1022:Flask of Crimson Tears +11, goods:1025:Flask of Crimson Tears +12, goods:1024:Flask of Crimson Tears +12, goods:1050:Flask of Cerulean Tears, goods:1051:Flask of Cerulean Tears, goods:1052:Flask of Cerulean Tears +1, goods:1053:Flask of Cerulean Tears +1, goods:1054:Flask of Cerulean Tears +2, goods:1055:Flask of Cerulean Tears +2, goods:1056:Flask of Cerulean Tears +3, goods:1057:Flask of Cerulean Tears +3, goods:1058:Flask of Cerulean Tears +4, goods:1059:Flask of Cerulean Tears +4, goods:1060:Flask of Cerulean Tears +5, goods:1061:Flask of Cerulean Tears +5, goods:1062:Flask of Cerulean Tears +6, goods:1063:Flask of Cerulean Tears +6, goods:1064:Flask of Cerulean Tears +7, goods:1065:Flask of Cerulean Tears +7
    assert t000001000_x8(goods7=1000)
    """State 5"""
    c1_110()
    while True:
        """State 1"""
        ClearTalkListData()
        """State 2"""
        # action:15000420:"Pass time"
        AddTalkListDataIf(not GetEventFlag(9411) or GetEventFlag(9412) == 1, 1, 15000420, -1)
        # action:15000540:"Level Up"
        AddTalkListData(2, 15000540, -1)
        # action:15000371:"Flasks"
        AddTalkListData(3, 15000371, -1)
        # action:15000390:"Memorize spell"
        AddTalkListData(4, 15000390, -1)
        # goods:250:Flask of Wondrous Physick, goods:251:Flask of Wondrous Physick, action:15000510:"Mix Wondrous Physick"
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 250, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 251, 2, 0, 0) == 1,
                          5, 15000510, -1)
        # action:15000511:""
        AddTalkListData(6, 15000511, -1)
        # action:15000395:"Sort chest"
        AddTalkListData(7, 15000395, -1)
        # action:15000520:"Great Runes"
        AddTalkListDataIf(f230(15) == 1, 8, 15000520, -1)
        # goods:8590:Whetstone Knife, action:15000530:"Ashes of War"
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 8590, 4, 1, 0) == 1, 9, 15000530, -1)
        # goods:8163:Tailoring Tools, goods:8188:Golden Tailoring Tools, action:22230000:"Alter garments"
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 8163, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 8188, 2, 0, 0) == 1,
                          10, 22230000, -1)

        # START MOD
        AddTalkListData(69, 69000000, -1) # Transmogrify Equipment
        # END MOD

        """State 25"""
        assert t000001000_x53()
        """State 15"""
        # action:20000009:"Leave"
        AddTalkListData(99, 20000009, -1)
        """State 6"""
        SetEventFlag(4652, 0)
        SetEventFlag(4656, 0)
        SetEventFlag(4654, 0)
        SetEventFlag(4655, 0)
        """State 3"""
        c1_140(1)
        ShowShopMessage(1)
        assert not (CheckSpecificPersonMenuIsOpen(1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        """State 8"""
        if GetTalkListEntryResult() == 1:
            """State 18"""
            c1_140(0)
            c1_110()
            def ExitPause():
                c1_110()
            assert t000001000_x34()
        elif GetTalkListEntryResult() == 2:
            """State 19"""
            assert t000001000_x35()
        elif GetTalkListEntryResult() == 3:
            """State 24"""
            c1_140(0)
            c1_110()
            def ExitPause():
                c1_110()
            assert t000001000_x48()
        elif GetTalkListEntryResult() == 4:
            """State 7"""
            OpenMagicEquip(-1, -1)
            assert not (CheckSpecificPersonMenuIsOpen(11, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 5:
            """State 12"""
            OpenPhysickMenu()
            assert not (CheckSpecificPersonMenuIsOpen(21, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 6:
            """State 40,41"""
            OpenTranspositionShop(105000, 105200)
            assert not (CheckSpecificPersonMenuIsOpen(18, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 7:
            """State 9"""
            OpenRepository()
            assert not (CheckSpecificPersonMenuIsOpen(3, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 8:
            """State 13"""
            c1_137()
            assert not (CheckSpecificPersonMenuIsOpen(24, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 9:
            """State 14"""
            OpenEquipmentChangeOfPurposeShop()
            assert not (CheckSpecificPersonMenuIsOpen(7, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 10:
            """State 16"""
            OpenTailoringShop(111000, 111399)
            assert not (CheckSpecificPersonMenuIsOpen(26, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 11:
            """State 20"""
            assert t000001000_x37(z6=4655)
        elif GetTalkListEntryResult() == 12:
            """State 27"""
            assert t000001000_x37(z6=4657)
        elif GetTalkListEntryResult() == 15:
            """State 26"""
            assert t000001000_x55()
        elif GetTalkListEntryResult() == 32:
            """State 22"""
            assert t000001000_x39()
        elif GetTalkListEntryResult() == 41 and GetEventFlag(120) == 1 and GetEventFlag(11102790) == 1:
            """State 23"""
            assert t000001000_x43()

        # START MOD
        elif GetTalkListEntryResult() == 69:
            OpenTailoringShop(4000000, 4200000)
            assert not (CheckSpecificPersonMenuIsOpen(26, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
            continue
        # END MOD

        else:
            """State 4,28"""
            return 0

def t000001000_x33():
    """State 0,1"""
    if GetEventFlag(1054532702) == 1:
        """State 2,4"""
        Label('L0')
        """State 3"""
        SetWorkValue(0, 0)
    elif GetEventFlag(4698) == 1:
        """State 8"""
        Goto('L0')
    elif GetEventFlag(11102790) == 1:
        """State 6,7"""
        SetWorkValue(0, 30)
    else:
        """State 5,9"""
        assert t000001000_x40()
    """State 10"""
    return 0

def t000001000_x34():
    """State 0,5"""
    CloseShopMessage()
    while True:
        """State 1"""
        ClearTalkListData()
        """State 2"""
        # action:15000430:"Until morning"
        AddTalkListData(1, 15000430, -1)
        # action:15000440:"Until noon"
        AddTalkListData(2, 15000440, -1)
        # action:15000450:"Until nightfall"
        AddTalkListData(3, 15000450, -1)
        # action:15000460:"Cancel"
        AddTalkListData(99, 15000460, -1)
        """State 3"""
        ShowShopMessage(1)
        assert not (CheckSpecificPersonMenuIsOpen(1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        """State 6"""
        if GetTalkListEntryResult() == 1:
            """State 7"""
            def ExitPause():
                c1_133(0.5)
            assert t000001000_x46(val2=0)
        elif GetTalkListEntryResult() == 2:
            """State 8"""
            def ExitPause():
                c1_133(0.5)
            assert t000001000_x46(val2=1)
        elif GetTalkListEntryResult() == 3:
            """State 9"""
            def ExitPause():
                c1_133(0.5)
            assert t000001000_x46(val2=2)
        else:
            """State 4,10"""
            return 0

def t000001000_x35():
    """State 0"""
    if not GetEventFlag(4651):
        """State 3,4"""
        OpenSoul()
        assert not (CheckSpecificPersonMenuIsOpen(10, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
    else:
        """State 2,1"""
        SetEventFlag(4652, 1)
        SetEventFlag(4656, 1)
        """State 5"""
        assert not GetEventFlag(4652)
        """State 6"""
        SetWorkValue(0, 0)
    """State 7"""
    return 0

def t000001000_x36(z7=_, val4=_, z8=1, z9=2, z10=60):
    """State 0,4"""
    assert not DoesPlayerHaveSpEffect(9600) and not DoesPlayerHaveSpEffect(9603)
    """State 1"""
    c1_128(2180, 21800000, 1, 3000, z7, z8, z9, z10)
    """State 2"""
    SetEventFlag(4651, 1)
    """State 3"""
    assert GetCurrentStateElapsedTime() > val4
    """State 5"""
    return 0

def t000001000_x37(z6=_):
    """State 0"""
    if not GetEventFlag(4651):
        """State 1,8"""
        assert t000001000_x29()
    else:
        """State 4"""
        pass
    """State 3"""
    SetEventFlag(4652, 1)
    SetEventFlag(z6, 1)
    """State 2"""
    assert (not GetEventFlag(4652) or (GetCurrentStateElapsedTime() > 5 and not DoesPlayerHaveSpEffect(9600)
            and not DoesPlayerHaveSpEffect(9603)))
    """State 5"""
    if not GetEventFlag(4680):
        """State 6,9"""
        assert t000001000_x30(z11=20010)
    else:
        """State 7"""
        pass
    """State 10"""
    return 0

def t000001000_x38():
    """State 0,1"""
    SetEventFlag(1054539200, 1)
    """State 4"""
    assert t000001000_x29()
    """State 2"""
    SetEventFlag(4652, 1)
    """State 3"""
    assert not GetEventFlag(4652)
    """State 5"""
    return 0

def t000001000_x39():
    """State 0,1"""
    SetEventFlag(1054539201, 1)
    """State 2"""
    SetEventFlag(1054539205, 1)
    """State 3"""
    assert GetCurrentStateElapsedTime() > 1.5
    """State 5"""
    SetEventFlag(9000, 0)
    SetEventFlag(9020, 0)
    """State 4"""
    c1_138(-1)
    Quit()

def t000001000_x40():
    """State 0,1"""
    if not CompareRNGValue(0, 0) != -1:
        """State 3,4"""
        ShuffleRNGSeed(100)
    else:
        """State 2"""
        pass
    """State 5"""
    SetRNGSeed()
    """State 6"""
    if CompareRNGValue(3, 69) == 1:
        """State 7,8"""
        SetWorkValue(0, 0)
    elif CompareRNGValue(3, 99) == 1:
        """State 9,10"""
        SetWorkValue(0, 10)
    else:
        """State 11,12"""
        SetWorkValue(0, 0)
    """State 13"""
    return 0

def t000001000_x41():
    """State 0"""
    while True:
        """State 1"""
        if GetEventFlag(1054532702) == 1:
            """State 2,7"""
            # actionbutton:6100:"Touch grace", actionbutton:6101:"Rest at site of grace"
            call = t000001000_x66(actionbutton1=6100, actionbutton2=6101, val1=45, z1=-120)
            if call.Done():
                break
            elif not GetEventFlag(1054532702):
                pass
        elif GetEventFlag(11102790) == 1:
            """State 4,6"""
            # actionbutton:6102:"Rest at table of lost grace", actionbutton:6103:"Rest at table of lost grace"
            call = t000001000_x45(actionbutton3=6102, actionbutton4=6103, val3=45)
            if call.Done():
                break
            elif not GetEventFlag(11102790):
                pass
        else:
            """State 3,5"""
            # actionbutton:6100:"Touch grace", actionbutton:6101:"Rest at site of grace"
            call = t000001000_x42(actionbutton1=6100, actionbutton2=6101)
            if call.Done():
                break
            elif GetEventFlag(1054532702) == 1 or GetEventFlag(11102790) == 1:
                pass
    """State 8"""
    return 0

def t000001000_x42(actionbutton1=_, actionbutton2=_):
    """State 0,1"""
    if CompareBonfireLevel(0, 0) == 1:
        """State 2,4"""
        assert t000001000_x3(actionbutton1=actionbutton1, flag3=6001, flag4=6000)
    else:
        """State 3,5"""
        assert t000001000_x3(actionbutton1=actionbutton2, flag3=6001, flag4=6000)
    """State 6"""
    return 0

def t000001000_x43():
    """State 0,7"""
    # action:20011080:"Begin Journey <?nextLoopCount?>?"
    call = t000001000_x0(action2=20011080)
    if call.Get() == 0:
        """State 2,8"""
        # action:20011081:"If you begin Journey <?nextLoopCount?>, you will not be able\nto return to the present world of Journey <?loopCount?>.\nBegin Journey <?nextLoopCount?>?"
        call = t000001000_x0(action2=20011081)
        if call.Get() == 0:
            """State 3,5"""
            SetEventFlag(3001, 1)
            """State 6"""
            Quit()
        elif call.Done():
            """State 4"""
            pass
    elif call.Done():
        """State 1"""
        pass
    """State 9"""
    return 0

def t000001000_x44(z2=11, z3=12):
    """State 0,2"""
    SetEventFlag(1042379200, 0)
    SetEventFlag(1042379202, 0)
    SetEventFlag(1042379206, 0)
    SetEventFlag(1046389201, 0)
    SetEventFlag(1043349250, 0)
    SetEventFlag(1038509250, 0)
    SetEventFlag(1043539250, 0)
    SetEventFlag(11009255, 0)
    SetEventFlag(1043509200, 0)
    SetEventFlag(1054559200, 0)
    SetEventFlag(1036489250, 0)
    SetEventFlag(1039519250, 0)
    SetEventFlag(1037519200, 0)
    SetEventFlag(10009656, 0)
    SetEventFlag(11009270, 0)
    SetEventFlag(11009275, 0)
    SetEventFlag(1054539210, 0)
    SetEventFlag(35009350, 0)
    SetEventFlag(35009352, 0)
    SetEventFlag(1054539215, 0)
    """State 4"""
    SetEventFlag(1042379208, 0)
    SetEventFlag(11009265, 0)
    SetEventFlag(35009355, 0)
    SetEventFlag(35009358, 0)
    if not GetEventFlag(953):
        """State 1"""
        if not GetEventFlag(4699):
            pass
        else:
            """State 8"""
            assert t000001000_x47(z3=z2)
    elif GetEventFlag(11102790) == 1:
        """State 3"""
        pass
    # eventflag:400001:lot:100010:Rold Medallion
    elif GetEventFlag(11009260) == 1 and not GetEventFlag(400001):
        """State 5"""
        pass
    elif GetEventFlag(108) == 1:
        """State 7"""
        pass
    elif GetEventFlag(110) == 1:
        """State 6"""
        pass
    else:
        """State 9"""
        assert t000001000_x50(z2=z2, z3=z3)
    """State 10"""
    return 0

def t000001000_x45(actionbutton3=6102, actionbutton4=6103, val3=45):
    """State 0"""
    if GetRelativeAngleBetweenPlayerAndSelf() < val3:
        """State 1"""
        Label('L0')
        """State 3"""
        call = t000001000_x42(actionbutton1=actionbutton3, actionbutton2=actionbutton4)
        if call.Done():
            """State 4"""
            return 0
        elif not GetRelativeAngleBetweenPlayerAndSelf() < val3:
            pass
    else:
        pass
    """State 2"""
    assert GetRelativeAngleBetweenPlayerAndSelf() < val3
    Goto('L0')

def t000001000_x46(val2=_):
    """State 0,8"""
    assert t000001000_x13()
    """State 4"""
    if not val2:
        """State 1"""
        c1_132(2)
        c1_134(0, 0, 0, 0, f219(), f220(), f221(), 2.5, 0.75, 2, 0, 0.75, 0.5)
    elif val2 == 1:
        """State 2"""
        c1_132(2)
        c1_134(0, 0, 0, 0, f222(), f223(), f224(), 2.5, 0.75, 2, 0, 0.75, 0.5)
    elif val2 == 2:
        """State 3"""
        c1_132(2)
        c1_134(0, 0, 0, 0, f225(), f226(), f227(), 2.5, 0.75, 2, 0, 0.75, 0.5)
    """State 5"""
    assert GetCurrentStateElapsedTime() > 0.8
    """State 9"""
    assert t000001000_x30(z11=0)
    """State 6"""
    assert not f233()
    """State 7"""
    assert GetCurrentStateElapsedTime() > 2.5
    """State 10"""
    return 0

def t000001000_x47(z3=_):
    """State 0,1"""
    # action:99990301:"Speak with Merina"
    AddTalkListData(z3, 99990301, -1)
    """State 2"""
    return 0

def t000001000_x48():
    """State 0,10"""
    assert t000001000_x49()
    """State 5"""
    CloseShopMessage()
    while True:
        """State 1"""
        ClearTalkListData()
        """State 2"""
        # action:15000370:"Add charge to flask"
        AddTalkListData(1, 15000370, -1)
        # action:15000380:"Increase amount replenished by flasks"
        AddTalkListData(2, 15000380, -1)
        # action:15000385:"Allocate flask charges"
        AddTalkListData(3, 15000385, -1)
        # action:15000372:"Cancel"
        AddTalkListData(99, 15000372, -1)
        """State 3"""
        ShowShopMessage(1)
        assert not (CheckSpecificPersonMenuIsOpen(1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        """State 6"""
        if GetTalkListEntryResult() == 1:
            """State 7"""
            # goods:1000:Flask of Crimson Tears, goods:1001:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1003:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1005:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1007:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1009:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1011:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1013:Flask of Crimson Tears +6, goods:1014:Flask of Crimson Tears +7, goods:1015:Flask of Crimson Tears +7, goods:1016:Flask of Crimson Tears +8, goods:1017:Flask of Crimson Tears +8, goods:1018:Flask of Crimson Tears +9, goods:1019:Flask of Crimson Tears +9, goods:1020:Flask of Crimson Tears +10, goods:1021:Flask of Crimson Tears +10, goods:1022:Flask of Crimson Tears +11, goods:1023:Flask of Crimson Tears +11, goods:1024:Flask of Crimson Tears +12, goods:1025:Flask of Crimson Tears +12, goods:10010:Golden Seed
            assert t000001000_x7(goods1=1000, goods2=10010)
        elif GetTalkListEntryResult() == 2:
            """State 8"""
            # goods:1000:Flask of Crimson Tears, goods:1001:Flask of Crimson Tears, goods:1002:Flask of Crimson Tears +1, goods:1003:Flask of Crimson Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1005:Flask of Crimson Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1007:Flask of Crimson Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1009:Flask of Crimson Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1011:Flask of Crimson Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1013:Flask of Crimson Tears +6, goods:1014:Flask of Crimson Tears +7, goods:1015:Flask of Crimson Tears +7, goods:1016:Flask of Crimson Tears +8, goods:1017:Flask of Crimson Tears +8, goods:1018:Flask of Crimson Tears +9, goods:1019:Flask of Crimson Tears +9, goods:1020:Flask of Crimson Tears +10, goods:1021:Flask of Crimson Tears +10, goods:1022:Flask of Crimson Tears +11, goods:1023:Flask of Crimson Tears +11, goods:1024:Flask of Crimson Tears +12, goods:1025:Flask of Crimson Tears +12, goods:10020:Sacred Tear
            assert t000001000_x6(goods3=1000, goods4=10020)
        elif GetTalkListEntryResult() == 3:
            """State 9"""
            # goods:1000:Flask of Crimson Tears, goods:1050:Flask of Cerulean Tears, goods:1001:Flask of Crimson Tears, goods:1051:Flask of Cerulean Tears, goods:1002:Flask of Crimson Tears +1, goods:1052:Flask of Cerulean Tears +1, goods:1003:Flask of Crimson Tears +1, goods:1053:Flask of Cerulean Tears +1, goods:1004:Flask of Crimson Tears +2, goods:1054:Flask of Cerulean Tears +2, goods:1005:Flask of Crimson Tears +2, goods:1055:Flask of Cerulean Tears +2, goods:1006:Flask of Crimson Tears +3, goods:1056:Flask of Cerulean Tears +3, goods:1007:Flask of Crimson Tears +3, goods:1057:Flask of Cerulean Tears +3, goods:1008:Flask of Crimson Tears +4, goods:1058:Flask of Cerulean Tears +4, goods:1009:Flask of Crimson Tears +4, goods:1059:Flask of Cerulean Tears +4, goods:1010:Flask of Crimson Tears +5, goods:1060:Flask of Cerulean Tears +5, goods:1011:Flask of Crimson Tears +5, goods:1061:Flask of Cerulean Tears +5, goods:1012:Flask of Crimson Tears +6, goods:1062:Flask of Cerulean Tears +6, goods:1013:Flask of Crimson Tears +6, goods:1063:Flask of Cerulean Tears +6, goods:1014:Flask of Crimson Tears +7, goods:1064:Flask of Cerulean Tears +7, goods:1015:Flask of Crimson Tears +7, goods:1065:Flask of Cerulean Tears +7, goods:1016:Flask of Crimson Tears +8, goods:1066:Flask of Cerulean Tears +8, goods:1017:Flask of Crimson Tears +8, goods:1067:Flask of Cerulean Tears +8, goods:1018:Flask of Crimson Tears +9, goods:1068:Flask of Cerulean Tears +9, goods:1019:Flask of Crimson Tears +9, goods:1069:Flask of Cerulean Tears +9, goods:1020:Flask of Crimson Tears +10, goods:1070:Flask of Cerulean Tears +10, goods:1021:Flask of Crimson Tears +10, goods:1071:Flask of Cerulean Tears +10, goods:1022:Flask of Crimson Tears +11, goods:1072:Flask of Cerulean Tears +11, goods:1023:Flask of Crimson Tears +11, goods:1073:Flask of Cerulean Tears +11, goods:1024:Flask of Crimson Tears +12, goods:1074:Flask of Cerulean Tears +12, goods:1025:Flask of Crimson Tears +12, goods:1075:Flask of Cerulean Tears +12
            assert t000001000_x16(goods5=1000)
        else:
            """State 4,11"""
            return 0

def t000001000_x49():
    """State 0"""
    if not GetEventFlag(720070) or not GetEventFlag(720080):
        """State 1,3"""
        SetEventFlag(720070, 1)
        SetEventFlag(720080, 1)
        """State 4"""
    else:
        """State 2"""
        pass
    """State 5"""
    return 0

def t000001000_x50(z2=11, z3=12):
    """State 0"""
    assert GetCurrentStateElapsedFrames() > 1
    """State 1"""
    assert t000001000_x58(z2=z2)
    """State 2"""
    assert t000001000_x59(z3=z3)
    """State 3"""
    return 0

def t000001000_x51():
    """State 0,9"""
    assert t000001000_x29()
    """State 7"""
    SetEventFlag(4652, 1)
    """State 4"""
    assert (not GetEventFlag(4652) or (GetCurrentStateElapsedTime() > 5 and not DoesPlayerHaveSpEffect(9600)
            and not DoesPlayerHaveSpEffect(9603)))
    """State 1"""
    if not GetEventFlag(4680):
        """State 2,8"""
        Label('L0')
        assert t000001000_x30(z11=20010) and GetCurrentStateElapsedTime() > 3
    elif GetEventFlag(108) == 1:
        """State 6"""
        Goto('L0')
    # eventflag:400001:lot:100010:Rold Medallion
    elif GetEventFlag(11009260) == 1 and not GetEventFlag(400001):
        """State 5"""
        Goto('L0')
    else:
        """State 3"""
        pass
    """State 10"""
    return 0

def t000001000_x52(z5=15):
    """State 0"""
    if not GetEventFlag(12019257):
        """State 1"""
        if not GetEventFlag(12019255):
            """State 5"""
            # action:21060900:"Talk to miniature Ranni"
            AddTalkListData(z5, 21060900, -1)
        elif not GetEventFlag(12019256):
            """State 6"""
            # action:21060901:"Talk to miniature Ranni"
            AddTalkListData(z5, 21060901, -1)
        else:
            """State 7"""
            # action:21060902:"Talk to miniature Ranni"
            AddTalkListData(z5, 21060902, -1)
    elif not GetEventFlag(12019260):
        """State 2"""
        if not GetEventFlag(12012711):
            """State 8"""
            # action:21060903:"Talk to Ranni"
            AddTalkListData(z5, 21060903, -1)
        elif GetEventFlag(1045379208) == 1:
            """State 9"""
            # action:21060910:"Talk to Ranni"
            AddTalkListData(z5, 21060910, -1)
        else:
            """State 13"""
            # action:21060911:"Talk to Ranni"
            AddTalkListData(z5, 21060911, -1)
    elif not GetEventFlag(12019265):
        """State 3"""
        if not GetEventFlag(12012712) or not GetEventFlag(1045379208):
            """State 10"""
            # action:21060912:"Talk to Ranni"
            AddTalkListData(z5, 21060912, -1)
        else:
            """State 11"""
            # action:21060920:"Talk to Ranni"
            AddTalkListData(z5, 21060920, -1)
    else:
        """State 4,12"""
        # action:21060921:"Talk to Ranni"
        AddTalkListData(z5, 21060921, -1)
    """State 14"""
    return 0

def t000001000_x53():
    """State 0"""
    # eventflag:400394:lot:103940:Miniature Ranni
    if (GetEventFlag(400394) == 1 and not GetEventFlag(12019280) and (GetEventFlag(12012710) == 1 or
        GetEventFlag(12012711) == 1 or GetEventFlag(12012712) == 1)):
        """State 4"""
        assert t000001000_x52(z5=15)
    # eventflag:400394:lot:103940:Miniature Ranni
    elif GetEventFlag(400394) == 1 and GetEventFlag(1034509406) == 1 and GetEventFlag(12012713) == 1:
        """State 5"""
        assert t000001000_x54(z4=15)
    else:
        """State 3"""
        assert t000001000_x44(z2=11, z3=12)
    """State 1"""
    # action:20010101:"Listen to the sounds of flame"
    AddTalkListDataIf(GetEventFlag(1054532702) == 1 and GetEventFlag(108) == 1 and not GetEventFlag(110),
                      32, 20010101, -1)
    # action:20010080:"Begin Journey <?nextLoopCount?>"
    AddTalkListDataIf(GetEventFlag(120) == 1 and GetEventFlag(11102790) == 1, 41, 20010080, -1)
    """State 6"""
    return 0

def t000001000_x54(z4=15):
    """State 0"""
    if not GetEventFlag(12019275):
        """State 1"""
        # action:21060930:"Talk to miniature Ranni"
        AddTalkListData(z4, 21060930, -1)
    else:
        """State 2"""
        # action:21060931:"Talk to Ranni"
        AddTalkListData(z4, 21060931, -1)
    """State 3"""
    return 0

def t000001000_x55():
    """State 0"""
    if not GetEventFlag(12012713):
        """State 1"""
        assert t000001000_x56()
    else:
        """State 2"""
        assert t000001000_x57()
    """State 3"""
    return 0

def t000001000_x56():
    """State 0"""
    if not GetEventFlag(12019257):
        """State 1"""
        if not GetEventFlag(12019255):
            """State 5"""
            # talk:10619000:"..."
            assert t000001000_x14(text2=10619000, z26=12019255, mode3=1)
        elif not GetEventFlag(12019256):
            """State 6"""
            # talk:10619100:"..."
            assert t000001000_x14(text2=10619100, z26=12019256, mode3=1)
        else:
            """State 7"""
            # talk:10619200:"..."
            call = t000001000_x14(text2=10619200, z26=12019257, mode3=1)
            if call.Done():
                pass
            elif call.Done():
                pass
    elif not GetEventFlag(12019260):
        """State 2"""
        if not GetEventFlag(12012711):
            """State 8"""
            # talk:10619300:"Perform for me a service, as recompense."
            assert t000001000_x15(text1=10619300, mode2=1)
        elif GetEventFlag(1045379208) == 1:
            """State 9"""
            # talk:10620000:"Let us speak of the past, a while."
            assert t000001000_x14(text2=10620000, z26=12019260, mode3=1)
        else:
            """State 10"""
            # talk:10603000:"Let us speak of the past, a while."
            assert t000001000_x14(text2=10603000, z26=12019260, mode3=1)
    elif not GetEventFlag(12019265):
        """State 3"""
        if not GetEventFlag(12012712) or not GetEventFlag(1045379208):
            """State 11"""
            # talk:10620100:"I turned my back on the Two Fingers and we have each been cursing the other since."
            assert t000001000_x15(text1=10620100, mode2=1)
        else:
            """State 12"""
            # talk:10621000:"Even when I turned my back upon the Two Fingers."
            assert t000001000_x14(text2=10621000, z26=12019265, mode3=1)
    else:
        """State 4,13"""
        # talk:10621100:"Ach, this form hath loosened my tongue."
        assert t000001000_x15(text1=10621100, mode2=1)
    """State 14"""
    return 0

def t000001000_x57():
    """State 0"""
    if not GetEventFlag(12019275):
        """State 3"""
        # talk:10625000:"..."
        assert t000001000_x14(text2=10625000, z26=12019275, mode3=1)
    else:
        """State 2"""
        # talk:10625100:"Mine will be an order not of gold, but the stars and moon of the chill night."
        assert t000001000_x15(text1=10625100, mode2=1)
    """State 1"""
    SetEventFlag(1034509407, 1)
    """State 4"""
    return 0

def t000001000_x58(z2=11):
    """State 0,7"""
    if not GetEventFlag(4680):
        """State 4"""
        if not GetEventFlag(1042372700):
            """State 2"""
            # action:21000001:"Talk to Melina"
            AddTalkListData(z2, 21000001, -1)
            SetEventFlag(1042379200, 1)
        elif GetEventFlag(4699) == 1:
            """State 31"""
            assert t000001000_x47(z3=z2)
        else:
            """State 5"""
            pass
    else:
        """State 6"""
        if (GetEventFlag(10009655) == 1 and not GetEventFlag(105) and not DoesPlayerHaveSpEffect(4270)
            and not DoesPlayerHaveSpEffect(4271) and not DoesPlayerHaveSpEffect(4272) and not DoesPlayerHaveSpEffect(4280)
            and not DoesPlayerHaveSpEffect(4282) and not DoesPlayerHaveSpEffect(4286)):
            """State 18"""
            # action:21000050:"Tell her you're off to Roundtable Hold"
            AddTalkListData(z2, 21000050, -1)
            SetEventFlag(10009656, 1)
        elif GetEventFlag(1042372701) == 1 and not GetEventFlag(1042379203) and not GetEventFlag(10009655):
            """State 3"""
            # action:21000002:"Talk to Melina"
            AddTalkListData(z2, 21000002, -1)
            SetEventFlag(1042379202, 1)
        elif GetEventFlag(1042372703) == 1:
            """State 28"""
            # action:21000004:"Talk to Melina"
            AddTalkListData(z2, 21000004, -1)
            SetEventFlag(1042379208, 1)
        elif (not GetEventFlag(1042372702) and not GetEventFlag(1042379207) and GetEventFlag(1042379203)
              == 1 and not GetEventFlag(10009655)):
            """State 8"""
            # action:21000003:"Talk to Melina"
            AddTalkListData(z2, 21000003, -1)
            SetEventFlag(1042379206, 1)
        elif GetEventFlag(1046382701) == 1 and not GetEventFlag(1046389202):
            """State 9"""
            SetEventFlag(1046389201, 1)
            # action:21000005:"Talk to Melina"
            AddTalkListDataIf(not GetEventFlag(1046382700), z2, 21000005, -1)
            # action:21000006:"Talk to Melina"
            AddTalkListDataIf(GetEventFlag(1046382700) == 1, z2, 21000006, -1)
        elif GetEventFlag(1043342700) == 1 and not GetEventFlag(1043349251):
            """State 10"""
            SetEventFlag(1043349250, 1)
            # action:21000007:"Talk to Melina"
            AddTalkListDataIf(not GetEventFlag(1046382700), z2, 21000007, -1)
            # action:21000008:"Talk to Melina"
            AddTalkListDataIf(GetEventFlag(1046382700) == 1, z2, 21000008, -1)
        elif GetEventFlag(1038502710) == 1 and not GetEventFlag(1038509251):
            """State 11"""
            SetEventFlag(1038509250, 1)
            # action:21000009:"Talk to Melina"
            AddTalkListDataIf(not GetEventFlag(1046382700), z2, 21000009, -1)
            # action:21000010:"Talk to Melina"
            AddTalkListDataIf(GetEventFlag(1046382700) == 1, z2, 21000010, -1)
        elif GetEventFlag(1043532710) == 1 and not GetEventFlag(1043539251):
            """State 12"""
            SetEventFlag(1043539250, 1)
            # action:21000011:"Talk to Melina"
            AddTalkListDataIf(not GetEventFlag(1046382700), z2, 21000011, -1)
            # action:21000012:"Talk to Melina"
            AddTalkListDataIf(GetEventFlag(1046382700) == 1, z2, 21000012, -1)
        elif GetEventFlag(11002740) == 1 and not GetEventFlag(11009256):
            """State 13"""
            SetEventFlag(11009255, 1)
            # action:21000013:"Talk to Melina"
            AddTalkListDataIf(not GetEventFlag(1046382700), z2, 21000013, -1)
            # action:21000014:"Talk to Melina"
            AddTalkListDataIf(GetEventFlag(1046382700) == 1, z2, 21000014, -1)
        elif GetEventFlag(1043502700) == 1 and not GetEventFlag(1043509201):
            """State 25"""
            SetEventFlag(1043509200, 1)
            # action:21000015:"Talk to Melina"
            AddTalkListDataIf(not GetEventFlag(1046382700), z2, 21000015, -1)
            # action:21000016:"Talk to Melina"
            AddTalkListDataIf(GetEventFlag(1046382700) == 1, z2, 21000016, -1)
        elif GetEventFlag(1054552700) == 1 and not GetEventFlag(1054559201):
            """State 14"""
            SetEventFlag(1054559200, 1)
            # action:21000017:"Talk to Melina"
            AddTalkListDataIf(not GetEventFlag(1046382700), z2, 21000017, -1)
            # action:21000018:"Talk to Melina"
            AddTalkListDataIf(GetEventFlag(1046382700) == 1, z2, 21000018, -1)
        # eventflag:400001:lot:100010:Rold Medallion
        elif (not GetEventFlag(1037519201) and not GetEventFlag(11009260) and not GetEventFlag(400001)
              and (GetEventFlag(1037512700) == 1 or GetEventFlag(1038512700) == 1 or GetEventFlag(1038502711)
              == 1 or GetEventFlag(1039512711) == 1 or GetEventFlag(1037522700) == 1)):
            """State 17"""
            SetEventFlag(1037519200, 1)
            # action:21000022:"Talk to Melina"
            AddTalkListData(z2, 21000022, -1)
        elif (GetEventFlag(1036482710) == 1 and GetEventFlag(1036489213) == 1 and GetEventFlag(3940)
              == 1 and not GetEventFlag(1036489251)):
            """State 15"""
            SetEventFlag(1036489250, 1)
            # action:21000020:"Talk to Melina"
            AddTalkListData(z2, 21000020, -1)
        elif (GetEventFlag(1039512710) == 1 and GetEventFlag(1039519209) == 1 and GetEventFlag(1036489210)
              == 1 and GetEventFlag(3940) == 1 and not GetEventFlag(1039519251)):
            """State 16"""
            SetEventFlag(1039519250, 1)
            # action:21000021:"Talk to Melina"
            AddTalkListData(z2, 21000021, -1)
        elif GetEventFlag(11002745) == 1:
            """State 29"""
            # action:21000023:"Talk to Melina"
            AddTalkListData(z2, 21000023, -1)
            SetEventFlag(11009265, 1)
        elif ((GetEventFlag(1054552701) == 1 or GetEventFlag(1052562700) == 1 or GetEventFlag(1052542700)
              == 1 or GetEventFlag(1051532700) == 1 or GetEventFlag(1052532710) == 1) and not GetEventFlag(1054539211)):
            """State 21"""
            SetEventFlag(1054539210, 1)
            # action:21000026:"Talk to Melina"
            AddTalkListData(z2, 21000026, -1)
        elif GetEventFlag(1054532702) == 1:
            """State 22"""
            SetEventFlag(1054539215, 1)
            # action:21000027:"Talk to Melina"
            AddTalkListData(z2, 21000027, -1)
        elif (GetEventFlag(35002730) == 1 or GetEventFlag(35002731) == 1) and not GetEventFlag(35009351):
            """State 23"""
            SetEventFlag(35009350, 1)
            # action:21000028:"Talk to Melina"
            AddTalkListData(z2, 21000028, -1)
        elif GetEventFlag(35002731) == 1 and not GetEventFlag(35009353):
            """State 24"""
            SetEventFlag(35009352, 1)
            # action:21000030:"Talk to Melina"
            AddTalkListData(z2, 21000030, -1)
        elif GetEventFlag(35002732) == 1 and not GetEventFlag(35002733):
            """State 26"""
            # action:21000029:"Talk to Melina"
            AddTalkListData(z2, 21000029, -1)
            SetEventFlag(35009355, 1)
        elif GetEventFlag(35002733) == 1:
            """State 27"""
            # action:21000031:"Talk to Melina"
            AddTalkListData(z2, 21000031, -1)
            SetEventFlag(35009358, 1)
        elif GetEventFlag(11109387) == 1 and not GetEventFlag(11009271):
            """State 19"""
            SetEventFlag(11009270, 1)
            # action:21000024:"Talk to Melina"
            AddTalkListData(z2, 21000024, -1)
        elif GetEventFlag(1049539207) == 1 and not GetEventFlag(11009276):
            """State 20"""
            SetEventFlag(11009275, 1)
            # action:21000025:"Talk to Melina"
            AddTalkListData(z2, 21000025, -1)
        elif GetEventFlag(4699) == 1:
            """State 30"""
            assert t000001000_x47(z3=z2)
        else:
            """State 1"""
            pass
    """State 32"""
    return 0

def t000001000_x59(z3=12):
    """State 0,5"""
    if not GetEventFlag(4680):
        """State 2"""
        if GetEventFlag(4699) == 1:
            """State 7"""
            assert t000001000_x47(z3=z3)
        else:
            """State 3"""
            pass
    else:
        """State 4"""
        if GetEventFlag(4699) == 1:
            """State 6"""
            assert t000001000_x47(z3=z3)
        else:
            """State 1"""
            pass
    """State 8"""
    return 0

def t000001000_x60():
    """State 0"""
    if GetEventFlag(953) == 1:
        """State 3,1"""
        SetEventFlag(4653, 1)
    else:
        """State 2"""
        pass
    """State 4"""
    return 0

def t000001000_x61():
    """State 0"""
    if (GetEventFlag(4681) == 1 and not GetEventFlag(10009655) and not DoesPlayerHaveSpEffect(4270) and
        not DoesPlayerHaveSpEffect(4271) and not DoesPlayerHaveSpEffect(4272) and not DoesPlayerHaveSpEffect(4280)
        and not DoesPlayerHaveSpEffect(4282) and not DoesPlayerHaveSpEffect(4286)):
        """State 2,3"""
        Label('L0')
        SetEventFlag(4653, 1)
    elif ((GetEventFlag(3062) == 1 or GetEventFlag(3063) == 1 or GetEventFlag(3064) == 1 or GetEventFlag(3065)
          == 1) and not GetEventFlag(10009655) and not DoesPlayerHaveSpEffect(4270) and not DoesPlayerHaveSpEffect(4271)
          and not DoesPlayerHaveSpEffect(4272) and not DoesPlayerHaveSpEffect(4280) and not DoesPlayerHaveSpEffect(4282)
          and not DoesPlayerHaveSpEffect(4286)):
        """State 4"""
        Goto('L0')
    # eventflag:400001:lot:100010:Rold Medallion
    elif (not GetEventFlag(11009260) and not GetEventFlag(400001) and not GetEventFlag(9104) and (GetEventFlag(11002741)
          == 1 or GetEventFlag(11002742) == 1 or GetEventFlag(11002743) == 1 or GetEventFlag(11002744)
          == 1)):
        """State 5"""
        Goto('L0')
    # eventflag:400001:lot:100010:Rold Medallion
    elif GetEventFlag(9104) == 1 and not GetEventFlag(400001):
        """State 6"""
        Goto('L0')
    else:
        """State 1"""
        pass
    """State 7"""
    return 0

def t000001000_x62():
    """State 0"""
    # eventflag:400001:lot:100010:Rold Medallion
    if GetEventFlag(9104) == 1 and not GetEventFlag(400001):
        """State 2,1"""
        SetEventFlag(4653, 1)
    else:
        """State 3"""
        pass
    """State 4"""
    return 0

def t000001000_x63():
    """State 0"""
    if not GetEventFlag(35009360):
        """State 3,1"""
        SetEventFlag(4653, 1)
    else:
        """State 2"""
        pass
    """State 4"""
    return 0

def t000001000_x64():
    """State 0"""
    while True:
        """State 1"""
        if CompareBonfireLevel(0, 0) == 1:
            """State 2"""
            # actionbutton:6100:"Touch grace"
            assert t000001000_x5(actionbutton5=6100, flag1=6001, flag2=6000)
        else:
            """State 3"""
            # actionbutton:6101:"Rest at site of grace"
            assert t000001000_x3(actionbutton1=6101, flag3=6001, flag4=6000)
        """State 4"""
        # action:20011020:"Cannot rest at sites of grace right now"
        assert t000001000_x4(action1=20011020)

def t000001000_x65():
    """State 0,2"""
    if GetCurrentStateElapsedTime() > 0.8:
        """State 3"""
        GiveSpEffectToPlayer(9606)
        def WhilePaused():
            GiveSpEffectToPlayer(9606)
        def ExitPause():
            GiveSpEffectToPlayer(9606)
        if not f233():
            pass
        elif GetEventFlag(9001) == 1:
            """State 1"""
            Label('L0')
    elif GetEventFlag(9001) == 1:
        Goto('L0')
    """State 4"""
    return 0

def t000001000_x66(actionbutton1=6100, actionbutton2=6101, val1=45, z1=-120):
    """State 0"""
    if RelativeAngleBetweenTwoPlayers_SpecifyAxis(z1) < val1:
        """State 1"""
        Label('L0')
        """State 3"""
        call = t000001000_x42(actionbutton1=actionbutton1, actionbutton2=actionbutton2)
        if call.Done():
            """State 4"""
            return 0
        elif not RelativeAngleBetweenTwoPlayers_SpecifyAxis(z1) < val1:
            pass
    else:
        pass
    """State 2"""
    assert RelativeAngleBetweenTwoPlayers_SpecifyAxis(z1) < val1
    Goto('L0')

def t000001000_x67():
    """State 0,2"""
    if GetCurrentStateElapsedTime() > 0.8:
        """State 3"""
        GiveSpEffectToPlayer(9610)
        def WhilePaused():
            GiveSpEffectToPlayer(9610)
        def ExitPause():
            GiveSpEffectToPlayer(9610)
        if not f233():
            pass
        elif GetEventFlag(9001) == 1:
            """State 1"""
            Label('L0')
    elif GetEventFlag(9001) == 1:
        Goto('L0')
    """State 4"""
    return 0

def t000001000_x68():
    """State 0"""
    # eventflag:400001:lot:100010:Rold Medallion
    if GetEventFlag(400001) == 1 and not GetEventFlag(108) and not GetEventFlag(11002745):
        """State 1,3"""
        assert t000001000_x30(z11=20011)
    else:
        """State 2,4"""
        assert t000001000_x30(z11=20010)
    """State 5"""
    return 0

def t000001000_x69():
    """State 0"""
    if GetEventFlag(1054532702) == 1:
        """State 1"""
        if GetEventFlag(110) == 1 and not GetEventFlag(1054532703) and not GetEventFlag(9116):
            """State 3,7"""
            SetEventFlag(9000, 0)
            SetEventFlag(9020, 0)
            """State 6"""
            c1_138(-1)
            """State 5"""
            SetEventFlag(1054539206, 1)
            assert GetCurrentStateElapsedTime() > 15
            """State 8"""
            c5_138(not GetEventFlag(11102790), 1001000)
            SetEventFlag(9000, 1)
            SetEventFlag(9020, 1)
            SetEventFlag(1054539206, 0)
        else:
            """State 4"""
            pass
    else:
        """State 2"""
        pass
    """State 9"""
    return 0

