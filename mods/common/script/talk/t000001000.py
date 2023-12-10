# -*- coding: utf-8 -*-
def t000001000_x10069():
    while True:
        CloseShopMessage()
        ClearTalkListData()
        AddTalkListData(1, 69000010, -1) # Head
        AddTalkListData(2, 69000011, -1) # Body
        AddTalkListData(3, 69000012, -1) # Arms
        AddTalkListData(4, 69000013, -1) # Legs
        AddTalkListData(5, 69000020, -1) # Untransmogrify equipped armor
        AddTalkListData(99, 15000372, -1) # Cancel
        ShowShopMessage(1)
        assert not (CheckSpecificPersonMenuIsOpen(1, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))

        if GetTalkListEntryResult() == 1:
            OpenRegularShop(4000000, 4099999)
            assert not (CheckSpecificPersonMenuIsOpen(5, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 2:
            OpenTailoringShop(4100000, 4199999)
            assert not (CheckSpecificPersonMenuIsOpen(26, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 3:
            OpenTailoringShop(4200000, 4299999)
            assert not (CheckSpecificPersonMenuIsOpen(26, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 4:
            OpenTailoringShop(4300000, 4399999)
            assert not (CheckSpecificPersonMenuIsOpen(26, 0) == 1 and not CheckSpecificPersonGenericDialogIsOpen(0))
        elif GetTalkListEntryResult() == 5:
            GiveSpEffectToPlayer(250)
        else:
            return 0
