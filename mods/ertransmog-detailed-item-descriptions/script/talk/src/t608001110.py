# -*- coding: utf-8 -*-
def t608001110_1():
    """State 0,1"""
    # actionbutton:6000:"Talk"
    t608001110_x5(flag1=6000, flag2=6000, flag3=6000, val1=5, val2=10, val3=12, val4=10, val5=12, actionbutton1=6000,
                  flag4=6000, flag5=6001, flag6=6000, flag7=6000, flag8=6000, z1=1, z2=1000000, z3=1000000,
                  z4=1000000, mode1=1, mode2=1)
    def WhilePaused():
        c1_139(0, 0)
    Quit()

def t608001110_1000():
    """State 0,2,3"""
    assert t608001110_x32()
    """State 1"""
    c1_120(1000)
    Quit()

def t608001110_2000():
    """State 0,2,3"""
    assert t608001110_x33()
    """State 1"""
    c1_120(2000)
    Quit()

def t608001110_x0(actionbutton1=_, flag5=6001, flag9=6000, flag10=6000, flag11=6000, flag12=6000, flag4=6000):
    """State 0"""
    while True:
        """State 1"""
        assert not GetOneLineHelpStatus() and not IsClientPlayer() and not IsPlayerDead() and not IsCharacterDisabled()
        """State 3"""
        assert (GetEventFlag(flag5) == 1 or GetEventFlag(flag9) == 1 or GetEventFlag(flag10) == 1 or
                GetEventFlag(flag11) == 1 or GetEventFlag(flag12) == 1)
        """State 4"""
        assert not GetEventFlag(flag4)
        """State 2"""
        if GetEventFlag(flag4) == 1:
            pass
        elif (not (not GetOneLineHelpStatus() and not IsClientPlayer() and not IsPlayerDead() and not
              IsCharacterDisabled())):
            pass
        elif (not GetEventFlag(flag5) and not GetEventFlag(flag9) and not GetEventFlag(flag10) and not
              GetEventFlag(flag11) and not GetEventFlag(flag12)):
            pass
        elif CheckActionButtonArea(actionbutton1):
            break
    """State 5"""
    return 0

def t608001110_x1():
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

def t608001110_x2(action1=26081000):
    """State 0,1"""
    # action:26081000:"Cannot use while entering combat"
    OpenGenericDialog(7, action1, 1, 0, 1)
    assert not CheckSpecificPersonGenericDialogIsOpen(0)
    """State 2"""
    return 0

def t608001110_x3(val2=10, val3=12):
    """State 0,1"""
    assert GetDistanceToPlayer() < val2 and GetCurrentStateElapsedFrames() > 1
    """State 2"""
    if not f228() and not f229():
        """State 3,6"""
        call = t608001110_x19()
        if call.Done():
            pass
        elif GetDistanceToPlayer() > val3 or GetTalkInterruptReason() == 6:
            """State 5"""
            assert t608001110_x1()
    else:
        """State 4,7"""
        call = t608001110_x29()
        if call.Done():
            pass
        elif GetDistanceToPlayer() > val3 or GetTalkInterruptReason() == 6:
            """State 8"""
            assert t608001110_x1()
    """State 9"""
    return 0

def t608001110_x4():
    """State 0,1"""
    assert t608001110_x1()
    """State 2"""
    return 0

def t608001110_x5(flag1=6000, flag2=6000, flag3=6000, val1=5, val2=10, val3=12, val4=10, val5=12, actionbutton1=6000,
                  flag4=6000, flag5=6001, flag6=6000, flag7=6000, flag8=6000, z1=1, z2=1000000, z3=1000000,
                  z4=1000000, mode1=1, mode2=1):
    """State 0"""
    assert GetCurrentStateElapsedTime() > 1.5
    while True:
        """State 2"""
        call = t608001110_x22(flag1=flag1, flag2=flag2, flag3=flag3, val1=val1, val2=val2, val3=val3,
                              val4=val4, val5=val5, actionbutton1=actionbutton1, flag4=flag4, flag5=flag5,
                              flag6=flag6, flag7=flag7, flag8=flag8, z1=z1, z2=z2, z3=z3, z4=z4, mode1=mode1,
                              mode2=mode2)
        assert IsClientPlayer() == 1
        """State 1"""
        call = t608001110_x21()
        assert not IsClientPlayer()
    """Unused"""
    """State 3"""
    return 0

def t608001110_x6(val1=5, val2=10, val3=12, val4=10, val5=12, actionbutton1=6000, flag4=6000, flag5=6001,
                  flag6=6000, flag7=6000, flag8=6000, z1=1, z2=1000000, z3=1000000, z4=1000000, mode1=1,
                  mode2=1):
    """State 0"""
    while True:
        """State 2"""
        call = t608001110_x9(actionbutton1=actionbutton1, flag4=flag4, flag5=flag5, z2=z2, z3=z3, z4=z4)
        def WhilePaused():
            RemoveMyAggroIf(IsAttackedBySomeone() == 1 and (DoesSelfHaveSpEffect(9626) == 1 and DoesSelfHaveSpEffect(9627) == 1))
            GiveSpEffectToPlayerIf(not CheckSpecificPersonTalkHasEnded(0), 9640)
        if call.Done():
            """State 4"""
            Label('L0')
            c1_138(1000000)
            call = t608001110_x13(val1=val1, z1=z1)
            def WhilePaused():
                c5_138(GetDistanceToPlayer() > 2.5, -1)
                RemoveMyAggroIf(IsAttackedBySomeone() == 1 and (DoesSelfHaveSpEffect(9626) == 1 and DoesSelfHaveSpEffect(9627) == 1))
                GiveSpEffectToPlayer(9640)
                c5_139(1 == (mode1 == 1), -1, 0)
                c5_139(1 == (mode2 == 1), 0, -1)
            def ExitPause():
                c1_138(-1)
            if call.Done():
                continue
            elif IsAttackedBySomeone() == 1:
                pass
        elif IsAttackedBySomeone() == 1 and not DoesSelfHaveSpEffect(9626) and not DoesSelfHaveSpEffect(9627):
            pass
        elif GetEventFlag(flag8) == 1:
            Goto('L0')
        elif GetEventFlag(flag6) == 1 and not GetEventFlag(flag7) and GetDistanceToPlayer() < val4:
            """State 5"""
            call = t608001110_x15(val5=val5)
            if call.Done():
                continue
            elif IsAttackedBySomeone() == 1:
                pass
        elif ((GetDistanceToPlayer() > val5 or GetTalkInterruptReason() == 6) and not CheckSpecificPersonTalkHasEnded(0)
              and not DoesSelfHaveSpEffect(9625)):
            """State 6"""
            assert t608001110_x26() and CheckSpecificPersonTalkHasEnded(0) == 1
            continue
        elif GetEventFlag(9000) == 1:
            """State 1"""
            assert not GetEventFlag(9000)
            continue
        """State 3"""
        def ExitPause():
            RemoveMyAggro()
        assert t608001110_x11(val2=val2, val3=val3)
    """Unused"""
    """State 7"""
    return 0

def t608001110_x7(val2=10, val3=12):
    """State 0,1"""
    call = t608001110_x17(val2=val2, val3=val3)
    assert IsPlayerDead() == 1
    """State 2"""
    t608001110_x3(val2=val2, val3=val3)
    Quit()
    """Unused"""
    """State 3"""
    return 0

def t608001110_x8(flag1=6000, val2=10, val3=12):
    """State 0,8"""
    assert t608001110_x31()
    """State 1"""
    if GetEventFlag(flag1) == 1:
        """State 2"""
        pass
    else:
        """State 3"""
        if GetDistanceToPlayer() < val2:
            """State 4,6"""
            call = t608001110_x20()
            if call.Done():
                pass
            elif GetDistanceToPlayer() > val3 or GetTalkInterruptReason() == 6:
                """State 7"""
                assert t608001110_x1()
        else:
            """State 5"""
            pass
    """State 9"""
    return 0

def t608001110_x9(actionbutton1=6000, flag4=6000, flag5=6001, z2=1000000, z3=1000000, z4=1000000):
    """State 0,1"""
    call = t608001110_x10(z5=2000, val6=2000)
    if call.Get() == 1:
        """State 2"""
        assert (t608001110_x0(actionbutton1=actionbutton1, flag5=flag5, flag9=6000, flag10=6000, flag11=6000,
                flag12=6000, flag4=flag4))
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x10(z5=_, val6=_):
    """State 0,1"""
    if f203(z5) == 1:
        """State 2"""
        assert GetCurrentStateElapsedFrames() > 1
        """State 4"""
        def WhilePaused():
            c1_119(z5)
        assert f202() == val6
        """State 5"""
        return 0
    else:
        """State 3,6"""
        return 1

def t608001110_x11(val2=10, val3=12):
    """State 0"""
    assert GetCurrentStateElapsedFrames() > 1
    """State 5"""
    assert t608001110_x1()
    """State 3"""
    if GetDistanceToPlayer() < val2:
        """State 1"""
        if IsPlayerAttacking() == 1:
            """State 6"""
            call = t608001110_x12()
            if call.Done():
                pass
            elif GetDistanceToPlayer() > val3 or GetTalkInterruptReason() == 6:
                """State 7"""
                assert t608001110_x27()
        else:
            """State 4"""
            pass
    else:
        """State 2"""
        pass
    """State 8"""
    return 0

def t608001110_x12():
    """State 0,1"""
    assert t608001110_x10(z5=1101, val6=1101)
    """State 2"""
    return 0

def t608001110_x13(val1=5, z1=1):
    """State 0,2"""
    assert t608001110_x23()
    """State 1"""
    call = t608001110_x14()
    if call.Done():
        pass
    elif (GetDistanceToPlayer() > val1 or GetTalkInterruptReason() == 6) and not DoesSelfHaveSpEffect(9625):
        """State 3"""
        assert t608001110_x25()
    """State 4"""
    return 0

def t608001110_x14():
    """State 0,1"""
    assert t608001110_x10(z5=1000, val6=1000)
    """State 2"""
    return 0

def t608001110_x15(val5=12):
    """State 0,1"""
    call = t608001110_x16()
    if call.Done():
        pass
    elif GetDistanceToPlayer() > val5 or GetTalkInterruptReason() == 6:
        """State 2"""
        assert t608001110_x26()
    """State 3"""
    return 0

def t608001110_x16():
    """State 0,1"""
    assert t608001110_x10(z5=1100, val6=1100)
    """State 2"""
    return 0

def t608001110_x17(val2=10, val3=12):
    """State 0,5"""
    assert t608001110_x31()
    """State 2"""
    assert not GetEventFlag(3000)
    while True:
        """State 1"""
        assert GetDistanceToPlayer() < val2
        """State 3"""
        call = t608001110_x18()
        if call.Done():
            pass
        elif GetDistanceToPlayer() > val3 or GetTalkInterruptReason() == 6:
            """State 4"""
            assert t608001110_x28()
    """Unused"""
    """State 6"""
    return 0

def t608001110_x18():
    """State 0,2"""
    call = t608001110_x10(z5=1102, val6=1102)
    if call.Get() == 1:
        """State 1"""
        Quit()
    elif call.Done():
        """State 3"""
        return 0

def t608001110_x19():
    """State 0,1"""
    assert t608001110_x10(z5=1001, val6=1001)
    """State 2"""
    return 0

def t608001110_x20():
    """State 0,1"""
    assert t608001110_x10(z5=1103, val6=1103)
    """State 2"""
    return 0

def t608001110_x21():
    """State 0"""
    Quit()
    """Unused"""
    """State 1"""
    return 0

def t608001110_x22(flag1=6000, flag2=6000, flag3=6000, val1=5, val2=10, val3=12, val4=10, val5=12, actionbutton1=6000,
                   flag4=6000, flag5=6001, flag6=6000, flag7=6000, flag8=6000, z1=1, z2=1000000, z3=1000000,
                   z4=1000000, mode1=1, mode2=1):
    """State 0"""
    while True:
        """State 1"""
        RemoveMyAggro()
        call = t608001110_x6(val1=val1, val2=val2, val3=val3, val4=val4, val5=val5, actionbutton1=actionbutton1,
                             flag4=flag4, flag5=flag5, flag6=flag6, flag7=flag7, flag8=flag8, z1=z1,
                             z2=z2, z3=z3, z4=z4, mode1=mode1, mode2=mode2)
        if CheckSelfDeath() == 1 or GetEventFlag(flag1) == 1:
            """State 3"""
            Label('L0')
            call = t608001110_x8(flag1=flag1, val2=val2, val3=val3)
            if not CheckSelfDeath() and not GetEventFlag(flag1):
                continue
            elif GetEventFlag(9000) == 1:
                pass
        elif GetEventFlag(flag2) == 1 or GetEventFlag(flag3) == 1:
            """State 2"""
            call = t608001110_x7(val2=val2, val3=val3)
            if CheckSelfDeath() == 1 or GetEventFlag(flag1) == 1:
                Goto('L0')
            elif not GetEventFlag(flag2) and not GetEventFlag(flag3):
                continue
            elif GetEventFlag(9000) == 1:
                pass
        elif GetEventFlag(9000) == 1 or (IsPlayerDead() == 1 and not DoesSelfHaveSpEffect(9649)):
            pass
        """State 4"""
        assert t608001110_x30() and (not GetEventFlag(9000) and not IsPlayerDead())
    """Unused"""
    """State 5"""
    return 0

def t608001110_x23():
    """State 0,1"""
    assert t608001110_x24()
    """State 2"""
    return 0

def t608001110_x24():
    """State 0,1"""
    assert t608001110_x10(z5=1104, val6=1104)
    """State 2"""
    return 0

def t608001110_x25():
    """State 0,1"""
    call = t608001110_x10(z5=1201, val6=1201)
    if call.Get() == 1:
        """State 2"""
        assert t608001110_x4()
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x26():
    """State 0,1"""
    call = t608001110_x10(z5=1300, val6=1300)
    if call.Get() == 1:
        """State 2"""
        assert t608001110_x4()
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x27():
    """State 0,1"""
    call = t608001110_x10(z5=1301, val6=1301)
    if call.Get() == 1:
        """State 2"""
        assert t608001110_x4()
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x28():
    """State 0,1"""
    call = t608001110_x10(z5=1302, val6=1302)
    if call.Get() == 1:
        """State 2"""
        assert t608001110_x4()
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x29():
    """State 0,1"""
    assert t608001110_x10(z5=1002, val6=1002)
    """State 2"""
    return 0

def t608001110_x30():
    """State 0,1"""
    assert t608001110_x1()
    """State 2"""
    return 0

def t608001110_x31():
    """State 0,1"""
    if CheckSpecificPersonGenericDialogIsOpen(0) == 1:
        """State 2"""
        ForceCloseGenericDialog()
    else:
        pass
    """State 3"""
    if CheckSpecificPersonMenuIsOpen(-1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0):
        """State 4"""
        ForceCloseMenu()
    else:
        pass
    """State 5"""
    return 0

def t608001110_x32():
    """State 0"""
    if not GetEventFlag(2051) and not GetEventFlag(2052):
        """State 1,3"""
        assert t608001110_x34()
    else:
        """State 2,4"""
        # action:26081000:"Cannot use while entering combat"
        assert t608001110_x2(action1=26081000)
    """State 5"""
    return 0

def t608001110_x33():
    """State 0,1"""
    # actionbutton:6380:"Use mirror"
    assert (t608001110_x0(actionbutton1=6380, flag5=6001, flag9=6000, flag10=6000, flag11=6000, flag12=6000,
            flag4=6000))
    """State 2"""
    return 0

def t608001110_x34():
    """State 0"""
    while True:
        """State 2"""
        c1_110()
        ClearTalkListData()
        """State 5"""
        # action:26080000:"Apply cosmetics"
        AddTalkListData(1, 26080000, -1)

        # START MOD
        AddTalkListData(69, 69000000, -1) # Transmogrify Equipment
        # END MOD

        # action:26080001:"Leave"
        AddTalkListData(99, 26080001, -1)
        """State 4"""
        ShowShopMessage(1)
        """State 6"""
        assert not (CheckSpecificPersonMenuIsOpen(1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        """State 3"""
        if GetTalkListEntryResult() == 1:
            """State 1"""
            ClearTalkActionState()
            OpenCharaMakeMenu(1)
            TurnCharacterToFaceEntity(-1, 10000, -1, -1)
            assert GetCurrentStateElapsedFrames() > 1 and GetWhetherChrEventAnimHasEnded(10000) == 1
            """State 9"""
            def WhilePaused():
                GiveSpEffectToPlayer(9614)
            assert not (CheckSpecificPersonMenuIsOpen(30, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
            """State 8"""
            if DidYouDoSomethingInTheMenu(1) == 1:
                break
            else:
                pass

        # START MOD
        elif GetTalkListEntryResult() == 69:
            OpenTailoringShop(4000000, 4200000)
            assert not (CheckSpecificPersonMenuIsOpen(26, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
            continue
        # END MOD

        else:
            """State 7"""
            break
    """State 10"""
    return 0

