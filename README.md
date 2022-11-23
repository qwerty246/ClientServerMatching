������ ������ �������� ����������� ���������� ��������� �������, �������� �������� ����� ���� �������� ������� ����� � ������-��������� ������������.

������ ������� �� 5 �����������:
1) Client - ���������� ���������� (� ���������, ����� ������� ������ ������� ����������� ����������);
2) libClientlib.so - ������������ ���������� ��� ������� (����� ����������� �������� ���������� ����������. ���������� ��������������, ��� ���������� ����� �������������� � � ����������, � � Gui �����������);
3) Server - ��������� ����������;
4) libServerlib.so - ������������ ���������� ��� ������� (����� ����������� �������� ��������� ����������. � ����� �������� �������� ���������� � ����������, ����� ����� ���� ����������� ������ � ������� ������);
5) Tests - �����.



# �������� �������

Client
�������� ����� ��� �������. ������������ ������������, ������������� ��� �������� ���������� ��� ������ � ������. ���������� ������� � ��������� ������ �� ������� � ������� json � ������� ���������� �� ����� � ������������ � ���������� �������.

Core
����������� � ��������� � ������������ ����������. ������ ��� ���������� � �������������.

RequestHandler
������������� ������ ��� ������ � ��������. ����� ���������� �������� � ����������� ������, �������������� ��������� ���������� � �������������.

Server
�������� ������ �������.

Session
��������� ������� �� ������������� � ������ ���������� ��� ������ RequestHandler.

Balance
������ ������������ � ��������� �������� � ���.

Offer
�������� ���������� ������, ����, id ������������.

User
�������� BalanceUSD, BalanceRUB, �������� � ����������� ����������� ������������.




# ������

������ ���������� � ������� cmake, ������ ���������� �� ���������. ������ ������:

> cd ClientServerMatching
> mkdir build
> cd build
> cmake ..
> cmake --build .

� ����� ������ ���� �������� ���������� � ��������� �� ���� ClientServerMatching/bin/



# ������

��� ������� ������� ���������� ��������� Server.

��� ������� ������� ����� ��������� Client. ����� ������� �������� ���� � 4 ���������� ����������:
0) Exit - ��������� ���������;
1) Registration - ������������ ������ ������������. ��������� �������������������� ��� � ������;
2) Autorisation - ������������ ������������� ������������. ��������� ��� � ������;
3) Remove user - ������� ������������� ������������. ��������� ��� � ������.
��������� � �� ����� �������� ���� ������, �� ���������� ������� ������� ���� ������������� � ��� ���������� � ���.

��� �������� ����������� ��� ����������� ������������ �������� ����� ����. ����� ������ ������, � � ����� �� ������������� �� ����������:
0) Exit - ��������� ���������;

10) Show my balance - ������� �� ����� ������ ������������, ����� � ����� �������� ������ USD � RUB;
11) Show my active offers - ������� �� ����� ���������� � ���� ������������� ������� ������������ (id ������, ���������� USD, ���� � RUB);
12) Show my completed offers - ������� �� ����� ���������� � ���� ����������� � ��������� ������� ������������ (id ������, ���������� USD, ���� � RUB, ��� ������);
13) Show all sale offers - ������� �� ����� ���������� � ���� ������������� �������� ������ ������������� (��� ��������, ���������� USD, ���� � RUB). � ������� ��������� ������� ����������: ����� ������� ����, ����� ������ ���� � ������� ���� ����� ���� ����������� � �������;
14) Show all purchase offers - ���������� ��� ������� ������ �������������;

20) Replenish balance, USD - ��������� ������ ������������ �� ����� USD, ������� �� ��� ������ (�� ����� �������, �. �. � ������� ����� ����������� �������������� ������� ������������);
21) Replenish balance, RUB - ����������;
22) Remove balance, USD - ������� �������� � ������� ������������ �� ����� USD, ������� �� ��� ������;
23) Remove balance, RUB - ����������;

30) Sale USD - ������� ����������� � ������� USD. ����� ����������� ���������� ������� ����� ����� �������� �����������. ���� ����� �������, �� ��� ���������� ���������� ���������� ������ ��� ������������ ���������� ����� �� ���� ����������, � ��� ����������� �������. ���� � �������� ����������� ������, ��� ������ �������, � ���������� �������� ��������������� ����� �� ������ ����������� � ���������� ������ �� ������. ���� �� � ���������� ����������� ������, �� ��� ������ �������, � �������� ����� �������� ����� � ���������� � ������ ������������;
31) Purchase USD - ����������, �� ���������� �������� ����������� � �������;
32) Remove offer - ������� �������� ����������� ������������ �� id (��� ����� ������ � ������ 11).

� ����������� gtest � �������� ��������� ����������� ������������. � ����� � �������� ������ �� ������� �����������, ����������� � �������� �������������, ����� ���������� � ����� ������������ � ������ �������������, � ����� �������� � ����������� ����������� � ��������� ���������. ������, ��������� � �������, ���������� � ����� TestOfferMerging.SuccessfullMerging_MakeDialBetweenThreeUsers1.