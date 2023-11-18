# -*- coding: utf-8 -*-
def t608001110_1():
    """State 0,1"""
    # actionbutton:6000:"Talk"
    t608001110_x4(flag1=6000, flag2=6000, flag3=6000, val2=5, val3=10, val4=12, val5=10, val6=12, actionbutton1=6000,
                  flag4=6000, flag5=6001, flag6=6000, flag7=6000, flag8=6000, z1=1, z2=1000000, z3=1000000,
                  z4=1000000, mode1=1, mode2=1)
    def WhilePaused():
        c1_139(0, 0)
    Quit()

def t608001110_1000():
    """State 0,2,3"""
    assert t608001110_x31()
    """State 1"""
    c1_120(1000)
    Quit()

def t608001110_2000():
    """State 0,2,3"""
    assert t608001110_x32()
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

def t608001110_x2(val3=10, val4=12):
    """State 0,1"""
    assert GetDistanceToPlayer() < val3 and GetCurrentStateElapsedFrames() > 1
    """State 2"""
    if not f228() and not f229():
        """State 3,6"""
        call = t608001110_x18()
        if call.Done():
            pass
        elif GetDistanceToPlayer() > val4 or GetTalkInterruptReason() == 6:
            """State 5"""
            assert t608001110_x1()
    else:
        """State 4,7"""
        call = t608001110_x28()
        if call.Done():
            pass
        elif GetDistanceToPlayer() > val4 or GetTalkInterruptReason() == 6:
            """State 8"""
            assert t608001110_x1()
    """State 9"""
    return 0

def t608001110_x3():
    """State 0,1"""
    assert t608001110_x1()
    """State 2"""
    return 0

def t608001110_x4(flag1=6000, flag2=6000, flag3=6000, val2=5, val3=10, val4=12, val5=10, val6=12, actionbutton1=6000,
                  flag4=6000, flag5=6001, flag6=6000, flag7=6000, flag8=6000, z1=1, z2=1000000, z3=1000000,
                  z4=1000000, mode1=1, mode2=1):
    """State 0"""
    assert GetCurrentStateElapsedTime() > 1.5
    while True:
        """State 2"""
        call = t608001110_x21(flag1=flag1, flag2=flag2, flag3=flag3, val2=val2, val3=val3, val4=val4,
                              val5=val5, val6=val6, actionbutton1=actionbutton1, flag4=flag4, flag5=flag5,
                              flag6=flag6, flag7=flag7, flag8=flag8, z1=z1, z2=z2, z3=z3, z4=z4, mode1=mode1,
                              mode2=mode2)
        assert IsClientPlayer() == 1
        """State 1"""
        call = t608001110_x20()
        assert not IsClientPlayer()
    """Unused"""
    """State 3"""
    return 0

def t608001110_x5(val2=5, val3=10, val4=12, val5=10, val6=12, actionbutton1=6000, flag4=6000, flag5=6001,
                  flag6=6000, flag7=6000, flag8=6000, z1=1, z2=1000000, z3=1000000, z4=1000000, mode1=1,
                  mode2=1):
    """State 0"""
    while True:
        """State 2"""
        call = t608001110_x8(actionbutton1=actionbutton1, flag4=flag4, flag5=flag5, z2=z2, z3=z3, z4=z4)
        def WhilePaused():
            RemoveMyAggroIf(IsAttackedBySomeone() == 1 and (DoesSelfHaveSpEffect(9626) == 1 and DoesSelfHaveSpEffect(9627) == 1))
            GiveSpEffectToPlayerIf(not CheckSpecificPersonTalkHasEnded(0), 9640)
        if call.Done():
            """State 4"""
            Label('L0')
            c1_138(1000000)
            call = t608001110_x12(val2=val2, z1=z1)
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
        elif GetEventFlag(flag6) == 1 and not GetEventFlag(flag7) and GetDistanceToPlayer() < val5:
            """State 5"""
            call = t608001110_x14(val6=val6)
            if call.Done():
                continue
            elif IsAttackedBySomeone() == 1:
                pass
        elif ((GetDistanceToPlayer() > val6 or GetTalkInterruptReason() == 6) and not CheckSpecificPersonTalkHasEnded(0)
              and not DoesSelfHaveSpEffect(9625)):
            """State 6"""
            assert t608001110_x25() and CheckSpecificPersonTalkHasEnded(0) == 1
            continue
        elif GetEventFlag(9000) == 1:
            """State 1"""
            assert not GetEventFlag(9000)
            continue
        """State 3"""
        def ExitPause():
            RemoveMyAggro()
        assert t608001110_x10(val3=val3, val4=val4)
    """Unused"""
    """State 7"""
    return 0

def t608001110_x6(val3=10, val4=12):
    """State 0,1"""
    call = t608001110_x16(val3=val3, val4=val4)
    assert IsPlayerDead() == 1
    """State 2"""
    t608001110_x2(val3=val3, val4=val4)
    Quit()
    """Unused"""
    """State 3"""
    return 0

def t608001110_x7(flag1=6000, val3=10, val4=12):
    """State 0,8"""
    assert t608001110_x30()
    """State 1"""
    if GetEventFlag(flag1) == 1:
        """State 2"""
        pass
    else:
        """State 3"""
        if GetDistanceToPlayer() < val3:
            """State 4,6"""
            call = t608001110_x19()
            if call.Done():
                pass
            elif GetDistanceToPlayer() > val4 or GetTalkInterruptReason() == 6:
                """State 7"""
                assert t608001110_x1()
        else:
            """State 5"""
            pass
    """State 9"""
    return 0

def t608001110_x8(actionbutton1=6000, flag4=6000, flag5=6001, z2=1000000, z3=1000000, z4=1000000):
    """State 0,1"""
    call = t608001110_x9(z5=2000, val7=2000)
    if call.Get() == 1:
        """State 2"""
        assert (t608001110_x0(actionbutton1=actionbutton1, flag5=flag5, flag9=6000, flag10=6000, flag11=6000,
                flag12=6000, flag4=flag4))
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x9(z5=_, val7=_):
    """State 0,1"""
    if f203(z5) == 1:
        """State 2"""
        assert GetCurrentStateElapsedFrames() > 1
        """State 4"""
        def WhilePaused():
            c1_119(z5)
        assert f202() == val7
        """State 5"""
        return 0
    else:
        """State 3,6"""
        return 1

def t608001110_x10(val3=10, val4=12):
    """State 0"""
    assert GetCurrentStateElapsedFrames() > 1
    """State 5"""
    assert t608001110_x1()
    """State 3"""
    if GetDistanceToPlayer() < val3:
        """State 1"""
        if IsPlayerAttacking() == 1:
            """State 6"""
            call = t608001110_x11()
            if call.Done():
                pass
            elif GetDistanceToPlayer() > val4 or GetTalkInterruptReason() == 6:
                """State 7"""
                assert t608001110_x26()
        else:
            """State 4"""
            pass
    else:
        """State 2"""
        pass
    """State 8"""
    return 0

def t608001110_x11():
    """State 0,1"""
    assert t608001110_x9(z5=1101, val7=1101)
    """State 2"""
    return 0

def t608001110_x12(val2=5, z1=1):
    """State 0,2"""
    assert t608001110_x22()
    """State 1"""
    call = t608001110_x13()
    if call.Done():
        pass
    elif (GetDistanceToPlayer() > val2 or GetTalkInterruptReason() == 6) and not DoesSelfHaveSpEffect(9625):
        """State 3"""
        assert t608001110_x24()
    """State 4"""
    return 0

def t608001110_x13():
    """State 0,1"""
    assert t608001110_x9(z5=1000, val7=1000)
    """State 2"""
    return 0

def t608001110_x14(val6=12):
    """State 0,1"""
    call = t608001110_x15()
    if call.Done():
        pass
    elif GetDistanceToPlayer() > val6 or GetTalkInterruptReason() == 6:
        """State 2"""
        assert t608001110_x25()
    """State 3"""
    return 0

def t608001110_x15():
    """State 0,1"""
    assert t608001110_x9(z5=1100, val7=1100)
    """State 2"""
    return 0

def t608001110_x16(val3=10, val4=12):
    """State 0,5"""
    assert t608001110_x30()
    """State 2"""
    assert not GetEventFlag(3000)
    while True:
        """State 1"""
        assert GetDistanceToPlayer() < val3
        """State 3"""
        call = t608001110_x17()
        if call.Done():
            pass
        elif GetDistanceToPlayer() > val4 or GetTalkInterruptReason() == 6:
            """State 4"""
            assert t608001110_x27()
    """Unused"""
    """State 6"""
    return 0

def t608001110_x17():
    """State 0,2"""
    call = t608001110_x9(z5=1102, val7=1102)
    if call.Get() == 1:
        """State 1"""
        Quit()
    elif call.Done():
        """State 3"""
        return 0

def t608001110_x18():
    """State 0,1"""
    assert t608001110_x9(z5=1001, val7=1001)
    """State 2"""
    return 0

def t608001110_x19():
    """State 0,1"""
    assert t608001110_x9(z5=1103, val7=1103)
    """State 2"""
    return 0

def t608001110_x20():
    """State 0"""
    Quit()
    """Unused"""
    """State 1"""
    return 0

def t608001110_x21(flag1=6000, flag2=6000, flag3=6000, val2=5, val3=10, val4=12, val5=10, val6=12, actionbutton1=6000,
                   flag4=6000, flag5=6001, flag6=6000, flag7=6000, flag8=6000, z1=1, z2=1000000, z3=1000000,
                   z4=1000000, mode1=1, mode2=1):
    """State 0"""
    while True:
        """State 1"""
        RemoveMyAggro()
        call = t608001110_x5(val2=val2, val3=val3, val4=val4, val5=val5, val6=val6, actionbutton1=actionbutton1,
                             flag4=flag4, flag5=flag5, flag6=flag6, flag7=flag7, flag8=flag8, z1=z1,
                             z2=z2, z3=z3, z4=z4, mode1=mode1, mode2=mode2)
        if CheckSelfDeath() == 1 or GetEventFlag(flag1) == 1:
            """State 3"""
            Label('L0')
            call = t608001110_x7(flag1=flag1, val3=val3, val4=val4)
            if not CheckSelfDeath() and not GetEventFlag(flag1):
                continue
            elif GetEventFlag(9000) == 1:
                pass
        elif GetEventFlag(flag2) == 1 or GetEventFlag(flag3) == 1:
            """State 2"""
            call = t608001110_x6(val3=val3, val4=val4)
            if CheckSelfDeath() == 1 or GetEventFlag(flag1) == 1:
                Goto('L0')
            elif not GetEventFlag(flag2) and not GetEventFlag(flag3):
                continue
            elif GetEventFlag(9000) == 1:
                pass
        elif GetEventFlag(9000) == 1 or (IsPlayerDead() == 1 and not DoesSelfHaveSpEffect(9649)):
            pass
        """State 4"""
        assert t608001110_x29() and (not GetEventFlag(9000) and not IsPlayerDead())
    """Unused"""
    """State 5"""
    return 0

def t608001110_x22():
    """State 0,1"""
    assert t608001110_x23()
    """State 2"""
    return 0

def t608001110_x23():
    """State 0,1"""
    assert t608001110_x9(z5=1104, val7=1104)
    """State 2"""
    return 0

def t608001110_x24():
    """State 0,1"""
    call = t608001110_x9(z5=1201, val7=1201)
    if call.Get() == 1:
        """State 2"""
        assert t608001110_x3()
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x25():
    """State 0,1"""
    call = t608001110_x9(z5=1300, val7=1300)
    if call.Get() == 1:
        """State 2"""
        assert t608001110_x3()
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x26():
    """State 0,1"""
    call = t608001110_x9(z5=1301, val7=1301)
    if call.Get() == 1:
        """State 2"""
        assert t608001110_x3()
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x27():
    """State 0,1"""
    call = t608001110_x9(z5=1302, val7=1302)
    if call.Get() == 1:
        """State 2"""
        assert t608001110_x3()
    elif call.Done():
        pass
    """State 3"""
    return 0

def t608001110_x28():
    """State 0,1"""
    assert t608001110_x9(z5=1002, val7=1002)
    """State 2"""
    return 0

def t608001110_x29():
    """State 0,1"""
    assert t608001110_x1()
    """State 2"""
    return 0

def t608001110_x30():
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

def t608001110_x31():
    """State 0,1"""
    assert t608001110_x33()
    """State 2"""
    return 0

def t608001110_x32():
    """State 0,1"""
    # actionbutton:6380:"Use mirror"
    assert (t608001110_x0(actionbutton1=6380, flag5=6001, flag9=6000, flag10=6000, flag11=6000, flag12=6000,
            flag4=6000))
    """State 2"""
    return 0

def t608001110_x33():
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

        # goods:8163:Tailoring Tools, goods:8188:Golden Tailoring Tools, action:22230000:"Alter garments"
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 8163, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 8188, 2, 0, 0) == 1,
                          2, 22230000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 701000, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 702000, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 703000, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 704000, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 705000, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 706000, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 707000, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 708000, 2, 0, 0) == 1 or ComparePlayerInventoryNumber(3, 709000, 2, 0, 0) == 1,
                          3, 50000000, -1)
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
        elif GetTalkListEntryResult() == 2:
            """State 22"""
            OpenTailoringShop(111000, 111399)
            assert not (CheckSpecificPersonMenuIsOpen(26, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 3:
            """State 11"""
            assert t608001110_x900()

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

def t608001110_x900():
    """State 0"""
    Label('L0')
    """State 10"""
    assert GetCurrentStateElapsedTime() > 0.1
    """State 5"""
    while True:
        """State 1"""
        ClearTalkListData()
        c1_110()
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 701000, 2, 0, 0) == 1, 10, 50001000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 702000, 2, 0, 0) == 1, 20, 50002000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 703000, 2, 0, 0) == 1, 30, 50003000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 704000, 2, 0, 0) == 1, 40, 50004000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 705000, 2, 0, 0) == 1, 50, 50005000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 706000, 2, 0, 0) == 1, 60, 50006000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 707000, 2, 0, 0) == 1, 70, 50007000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 708000, 2, 0, 0) == 1, 80, 50008000, -1)
        AddTalkListDataIf(ComparePlayerInventoryNumber(3, 709000, 2, 0, 0) == 1, 90, 50009000, -1)
        # action:15000460:"Cancel"
        AddTalkListData(99, 15000460, -1)
        """State 3"""
        c1_140(1)
        ShowShopMessage(1)
        assert not (CheckSpecificPersonMenuIsOpen(1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        """State 8"""
        if GetTalkListEntryResult() == 10:
            """State 6"""
            assert (t608001110_x910(action1=50001000, val1=6901000, goods1=701010, goods2=701020, goods3=701030,
                    goods4=701040, goods5=701050, goods6=701060, goods7=701070, goods8=701080, goods9=701090,
                    goods10=701100, goods11=701110, goods12=701120))
        elif GetTalkListEntryResult() == 20:
            """State 7"""
            assert (t608001110_x910(action1=50002000, val1=6902000, goods1=702010, goods2=702020, goods3=702030,
                    goods4=702040, goods5=702050, goods6=702060, goods7=702070, goods8=702080, goods9=702090,
                    goods10=702100, goods11=702110, goods12=702120))
        elif GetTalkListEntryResult() == 30:
            """State 9"""
            assert (t608001110_x910(action1=50003000, val1=6903000, goods1=703010, goods2=703020, goods3=703030,
                    goods4=703040, goods5=703050, goods6=703060, goods7=703070, goods8=703080, goods9=703090,
                    goods10=703100, goods11=703110, goods12=703120))
        elif GetTalkListEntryResult() == 40:
            """State 11"""
            assert (t608001110_x910(action1=50004000, val1=6904000, goods1=704010, goods2=704020, goods3=704030,
                    goods4=704040, goods5=704050, goods6=704060, goods7=704070, goods8=704080, goods9=704090,
                    goods10=704100, goods11=704110, goods12=704120))
        elif GetTalkListEntryResult() == 50:
            """State 12"""
            assert (t608001110_x910(action1=50005000, val1=6905000, goods1=705010, goods2=705020, goods3=705030,
                    goods4=705040, goods5=705050, goods6=705060, goods7=705070, goods8=705080, goods9=705090,
                    goods10=705100, goods11=705110, goods12=705120))
        elif GetTalkListEntryResult() == 60:
            """State 13"""
            assert (t608001110_x910(action1=50006000, val1=6906000, goods1=706010, goods2=706020, goods3=706030,
                    goods4=706040, goods5=706050, goods6=706060, goods7=706070, goods8=706080, goods9=706090,
                    goods10=706100, goods11=706110, goods12=706120))
        else:
            """State 14"""
            return 0
    """Unused"""
    """State 2"""
    if GetEventFlag(1049630199) == 1:
        pass
    else:
        Goto('L0')
    """State 4"""
    return 0
    """State 15"""
    return 0

def t608001110_x910(action1=_, val1=_, goods1=_, goods2=_, goods3=_, goods4=_, goods5=_, goods6=_, goods7=_,
                    goods8=_, goods9=_, goods10=_, goods11=_, goods12=_):
    """State 0"""
    assert GetCurrentStateElapsedTime() > 0.1
    while True:
        """State 1"""
        c1_110()
        ClearTalkListData()
        """State 2"""
        if not goods1:
            pass
        else:
            """State 3"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods1), 10, action1 + 100010, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods1) == 1, 10, action1 + 100011, -1)
        """State 4"""
        if not goods2:
            pass
        else:
            """State 5"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods2), 20, action1 + 100020, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods2) == 1, 20, action1 + 100021, -1)
        """State 6"""
        if not goods3:
            pass
        else:
            """State 7"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods3), 30, action1 + 100030, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods3) == 1, 30, action1 + 100031, -1)
        """State 8"""
        if not goods4:
            pass
        else:
            """State 9"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods4), 40, action1 + 100040, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods4) == 1, 40, action1 + 100041, -1)
        """State 10"""
        if not goods5:
            pass
        else:
            """State 11"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods5), 50, action1 + 100050, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods5) == 1, 50, action1 + 100051, -1)
        """State 12"""
        if not goods6:
            pass
        else:
            """State 13"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods6), 60, action1 + 100060, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods6) == 1, 60, action1 + 100061, -1)
        """State 14"""
        if not goods7:
            pass
        else:
            """State 15"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods7), 70, action1 + 100070, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods7) == 1, 70, action1 + 100071, -1)
        """State 16"""
        if not goods8:
            pass
        else:
            """State 17"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods8), 80, action1 + 100080, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods8) == 1, 80, action1 + 100081, -1)
        """State 18"""
        if not goods9:
            pass
        else:
            """State 19"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods9), 90, action1 + 100090, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods9) == 1, 90, action1 + 100091, -1)
        """State 20"""
        if not goods10:
            pass
        else:
            """State 21"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods10), 100, action1 + 100100, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods10) == 1, 100, action1 + 100101, -1)
        """State 22"""
        if not goods11:
            pass
        else:
            """State 23"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods11), 110, action1 + 100110, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods11) == 1, 110, action1 + 100111, -1)
        """State 24"""
        if not goods12:
            pass
        else:
            """State 25"""
            AddTalkListDataIf(not DoesPlayerHaveItem(3, goods12), 120, action1 + 100120, -1)
            AddTalkListDataIf(DoesPlayerHaveItem(3, goods12) == 1, 120, action1 + 100121, -1)
        """State 26"""
        # action:15000460:"Cancel"
        AddTalkListData(99, 15000460, -1)
        ShowShopMessage(1)
        assert not (CheckSpecificPersonMenuIsOpen(1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        """State 27"""
        if GetTalkListEntryResult() == 10:
            """State 28"""
            if not DoesPlayerHaveItem(3, goods1):
                """State 29"""
                assert t608001110_x920(goods13=701010) == 0
                """State 30"""
                GiveSpEffectToPlayer(val1 + 19)
            else:
                """State 31"""
                assert t608001110_x920(goods13=701010) == 0
            """State 32"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 20:
            """State 33"""
            if not DoesPlayerHaveItem(3, goods2):
                """State 34"""
                assert t608001110_x920(goods13=701010) == 0
                """State 35"""
                GiveSpEffectToPlayer(val1 + 29)
            else:
                """State 36"""
                assert t608001110_x920(goods13=701010) == 0
            """State 37"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 30:
            """State 38"""
            if not DoesPlayerHaveItem(3, goods3):
                """State 39"""
                assert t608001110_x920(goods13=701010) == 0
                """State 40"""
                GiveSpEffectToPlayer(val1 + 39)
            else:
                """State 41"""
                assert t608001110_x920(goods13=701010) == 0
            """State 42"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 40:
            """State 43"""
            if not DoesPlayerHaveItem(3, goods4):
                """State 44"""
                assert t608001110_x920(goods13=701010) == 0
                """State 45"""
                GiveSpEffectToPlayer(val1 + 49)
            else:
                """State 46"""
                assert t608001110_x920(goods13=701010) == 0
            """State 47"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 50:
            """State 48"""
            if not DoesPlayerHaveItem(3, goods5):
                """State 49"""
                assert t608001110_x920(goods13=701010) == 0
                """State 50"""
                GiveSpEffectToPlayer(val1 + 59)
            else:
                """State 51"""
                assert t608001110_x920(goods13=701010) == 0
            """State 52"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 60:
            """State 53"""
            if not DoesPlayerHaveItem(3, goods6):
                """State 54"""
                assert t608001110_x920(goods13=701010) == 0
                """State 55"""
                GiveSpEffectToPlayer(val1 + 69)
            else:
                """State 56"""
                assert t608001110_x920(goods13=701010) == 0
            """State 57"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 70:
            """State 58"""
            if not DoesPlayerHaveItem(3, goods7):
                """State 59"""
                assert t608001110_x920(goods13=701010) == 0
                """State 60"""
                GiveSpEffectToPlayer(val1 + 79)
            else:
                """State 61"""
                assert t608001110_x920(goods13=701010) == 0
            """State 62"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 80:
            """State 63"""
            if not DoesPlayerHaveItem(3, goods8):
                """State 64"""
                assert t608001110_x920(goods13=701010) == 0
                """State 65"""
                GiveSpEffectToPlayer(val1 + 89)
            else:
                """State 66"""
                assert t608001110_x920(goods13=701010) == 0
            """State 67"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 90:
            """State 68"""
            if not DoesPlayerHaveItem(3, goods9):
                """State 69"""
                assert t608001110_x920(goods13=701010) == 0
                """State 70"""
                GiveSpEffectToPlayer(val1 + 99)
            else:
                """State 71"""
                assert t608001110_x920(goods13=701010) == 0
            """State 72"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 100:
            """State 73"""
            if not DoesPlayerHaveItem(3, goods10):
                """State 74"""
                assert t608001110_x920(goods13=701010) == 0
                """State 75"""
                GiveSpEffectToPlayer(val1 + 109)
            else:
                """State 76"""
                assert t608001110_x920(goods13=701010) == 0
            """State 77"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 110:
            """State 78"""
            if not DoesPlayerHaveItem(3, goods11):
                """State 79"""
                assert t608001110_x920(goods13=701010) == 0
                """State 80"""
                GiveSpEffectToPlayer(val1 + 119)
            else:
                """State 81"""
                assert t608001110_x920(goods13=701010) == 0
            """State 82"""
            assert GetCurrentStateElapsedTime() > 0.1
        elif GetTalkListEntryResult() == 120:
            """State 83"""
            if not DoesPlayerHaveItem(3, goods12):
                """State 84"""
                assert t608001110_x920(goods13=701010) == 0
                """State 85"""
                GiveSpEffectToPlayer(val1 + 129)
            else:
                """State 86"""
                assert t608001110_x920(goods13=701010) == 0
            """State 87"""
            assert GetCurrentStateElapsedTime() > 0.1
        else:
            """State 88"""
            return 0

def t608001110_x920(goods13=701010):
    """State 0"""
    PlayerEquipmentQuantityChange(3, goods13, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 10, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 20, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 30, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 40, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 50, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 60, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 70, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 80, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 90, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 100, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 110, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 120, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1000, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1010, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1020, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1030, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1040, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1050, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1060, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1070, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1080, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1090, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1100, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1110, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 1120, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2000, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2010, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2020, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2030, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2040, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2050, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2060, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2070, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2080, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2090, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2100, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2110, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 2120, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3000, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3010, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3020, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3030, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3040, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3050, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3060, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3070, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3080, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3090, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3100, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3110, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 3120, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4000, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4010, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4020, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4030, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4040, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4050, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4060, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4070, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4080, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4090, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4100, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4110, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 4120, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5000, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5010, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5020, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5030, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5040, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5050, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5060, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5070, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5080, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5090, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5100, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5110, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 5120, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6000, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6010, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6020, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6030, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6040, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6050, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6060, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6070, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6080, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6090, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6100, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6110, -1)
    PlayerEquipmentQuantityChange(3, goods13 + 6120, -1)
    return 0

