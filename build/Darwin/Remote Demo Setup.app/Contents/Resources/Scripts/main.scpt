FasdUAS 1.101.10   ��   ��    k             l      ��  ��   JD
 * This script is the glue that binds remote-jblend.app and the proximity
 * launcher together for the MIDP3 Remote Display Demo.  It offers to
 * establish an ad-hoc WiFi network, obtains the Bluetooth Mac and IP
 * addresses of the phone, and sets up the bluetooth proximity app to start
 * and kill the remote display.
      � 	 	� 
   *   T h i s   s c r i p t   i s   t h e   g l u e   t h a t   b i n d s   r e m o t e - j b l e n d . a p p   a n d   t h e   p r o x i m i t y 
   *   l a u n c h e r   t o g e t h e r   f o r   t h e   M I D P 3   R e m o t e   D i s p l a y   D e m o .     I t   o f f e r s   t o 
   *   e s t a b l i s h   a n   a d - h o c   W i F i   n e t w o r k ,   o b t a i n s   t h e   B l u e t o o t h   M a c   a n d   I P 
   *   a d d r e s s e s   o f   t h e   p h o n e ,   a n d   s e t s   u p   t h e   b l u e t o o t h   p r o x i m i t y   a p p   t o   s t a r t 
   *   a n d   k i l l   t h e   r e m o t e   d i s p l a y . 
     
  
 l     ��������  ��  ��        l     ����  r         m        �    1 3 0 1 1 3 0 1 1 3  o      ���� 
0 wepkey  ��  ��        l     ��������  ��  ��        l      ��  ��    P J
 * cachedir is where'll we'll copy the scripts needed by Proximity.app.
      �   � 
   *   c a c h e d i r   i s   w h e r e ' l l   w e ' l l   c o p y   t h e   s c r i p t s   n e e d e d   b y   P r o x i m i t y . a p p . 
        l    ����  r        m       �     ` $ H O M E / L i b r a r y / A p p l i c a t i o n \   S u p p o r t / R e m o t e D i s p l a y  o      ���� 0 cachedir  ��  ��     ! " ! l    #���� # I   �� $��
�� .sysoexecTEXT���     TEXT $ b     % & % m    	 ' ' � ( (  m k d i r   - p   & o   	 
���� 0 cachedir  ��  ��  ��   "  ) * ) l     ��������  ��  ��   *  + , + l    -���� - r     . / . n     0 1 0 1    ��
�� 
strq 1 l    2���� 2 n     3 4 3 1    ��
�� 
psxp 4 l    5���� 5 b     6 7 6 l    8���� 8 I   �� 9 :
�� .earsffdralis        afdr 9  f     : �� ;��
�� 
rtyp ; m    ��
�� 
ctxt��  ��  ��   7 m     < < � = = 6 C o n t e n t s : R e s o u r c e s : S c r i p t s :��  ��  ��  ��   / o      ���� 0 	scriptdir  ��  ��   ,  > ? > l     ��������  ��  ��   ?  @ A @ l    + B���� B I    +�� C��
�� .sysoexecTEXT���     TEXT C b     ' D E D b     % F G F b     # H I H m     ! J J � K K  c p   - f   I o   ! "���� 0 	scriptdir   G m   # $ L L � M M $ s t a r t - r e m o t e . s c p t   E o   % &���� 0 cachedir  ��  ��  ��   A  N O N l  , 9 P���� P I  , 9�� Q��
�� .sysoexecTEXT���     TEXT Q b   , 5 R S R b   , 3 T U T b   , / V W V m   , - X X � Y Y  c p   - f   W o   - .���� 0 	scriptdir   U m   / 2 Z Z � [ [ " k i l l - r e m o t e . s c p t   S o   3 4���� 0 cachedir  ��  ��  ��   O  \ ] \ l     ��������  ��  ��   ]  ^ _ ^ l      �� ` a��   ` � �
 * remote-jblend didn't used to have a quit handler, and Proximity.app doesn't
 * reliably respond to quit events, so resorted to shell commands to terminate
 * these apps.
     a � b b^ 
   *   r e m o t e - j b l e n d   d i d n ' t   u s e d   t o   h a v e   a   q u i t   h a n d l e r ,   a n d   P r o x i m i t y . a p p   d o e s n ' t 
   *   r e l i a b l y   r e s p o n d   t o   q u i t   e v e n t s ,   s o   r e s o r t e d   t o   s h e l l   c o m m a n d s   t o   t e r m i n a t e 
   *   t h e s e   a p p s . 
   _  c d c i      e f e I      �� g���� 0 killapp killApp g  h�� h o      ���� 0 appname appName��  ��   f k     	 i i  j k j l     �� l m��   l > 8 Have to set COMMAND_MODE=unix2003 to get ps -ef to work    m � n n p   H a v e   t o   s e t   C O M M A N D _ M O D E = u n i x 2 0 0 3   t o   g e t   p s   - e f   t o   w o r k k  o�� o I    	�� p��
�� .sysoexecTEXT���     TEXT p b      q r q b      s t s m      u u � v v X e x p o r t   C O M M A N D _ M O D E = u n i x 2 0 0 3 ;   p s   - e f   |   g r e p   t o    ���� 0 appname appName r m     w w � x x j   |   g r e p   - v   g r e p   |   a w k   ' { p r i n t   $ 2 } '   |   x a r g s   k i l l   - T E R M��  ��   d  y z y l     ��������  ��  ��   z  { | { l      �� } ~��   } . (
 * Make sure the demo is not running.
     ~ �   P 
   *   M a k e   s u r e   t h e   d e m o   i s   n o t   r u n n i n g . 
   |  � � � l  : B ����� � I   : B�� ����� 0 killapp killApp �  ��� � m   ; > � � � � �  r e m o t e - j b l e n d��  ��  ��  ��   �  � � � l  C K ����� � I   C K�� ����� 0 killapp killApp �  ��� � m   D G � � � � �  P r o x i m i t y��  ��  ��  ��   �  � � � l     ��������  ��  ��   �  � � � l      �� � ���   � W Q
 * Ensure support for assistive devices is enabled so we can do GUI scripting.
     � � � � � 
   *   E n s u r e   s u p p o r t   f o r   a s s i s t i v e   d e v i c e s   i s   e n a b l e d   s o   w e   c a n   d o   G U I   s c r i p t i n g . 
   �  � � � l  L \ ����� � O   L \ � � � r   R [ � � � 1   R W��
�� 
uien � o      ���� 0 
ui_enabled 
UI_enabled � m   L O � ��                                                                                  sevs  alis    �  
Grendel HD                 Ǟ6QH+     �System Events.app                                               *��7��        ����  	                CoreServices    Ǟ��      �8'7       �   H   G  8Grendel HD:System:Library:CoreServices:System Events.app  $  S y s t e m   E v e n t s . a p p   
 G r e n d e l   H D  -System/Library/CoreServices/System Events.app   / ��  ��  ��   �  � � � l  ] � ����� � Z   ] � � ����� � =  ] b � � � o   ] `���� 0 
ui_enabled 
UI_enabled � m   ` a��
�� boovfals � O   e � � � � k   k � � �  � � � I  k p������
�� .miscactvnull��� ��� null��  ��   �  � � � r   q � � � � 5   q |�� ���
�� 
xppb � m   u x � � � � � H c o m . a p p l e . p r e f e r e n c e . u n i v e r s a l a c c e s s
�� kfrmID   � 1   | ���
�� 
xpcp �  ��� � I  � ��� � �
�� .sysodlogaskr        TEXT � b   � � � � � b   � � � � � b   � � � � � m   � � � � � � � � T h i s   d e m o   u t i l i z e s   t h e   b u i l t - i n   G r a p h i c   U s e r   I n t e r f a c e   S c r i p t i n g   a r c h i t e c t u r e   o f   M a c   O S   X   w h i c h   i s   c u r r e n t l y   d i s a b l e d . � o   � ���
�� 
ret  � o   � ���
�� 
ret  � m   � � � � � � �\ A c t i v a t e   G U I   S c r i p t i n g   b y   p r e s s i n g   C a n c e l   a n d   t h e n   s e l e c t i n g   t h e   c h e c k b o x   " E n a b l e   a c c e s s   f o r   a s s i s t i v e   d e v i c e s "   i n   t h e   U n i v e r s a l   A c c e s s   p r e f e r e n c e   p a n e .   T h e n   r e - r u n   t h e   d e m o . � �� � �
�� 
disp � m   � �����  � �� � �
�� 
btns � J   � � � �  ��� � m   � � � � � � �  C a n c e l��   � �� ���
�� 
dflt � m   � ����� ��  ��   � m   e h � ��                                                                                  sprf  alis    v  
Grendel HD                 Ǟ6QH+     �System Preferences.app                                          + �k�        ����  	                Applications    Ǟ��      �kh       �  .Grendel HD:Applications:System Preferences.app  .  S y s t e m   P r e f e r e n c e s . a p p   
 G r e n d e l   H D  #Applications/System Preferences.app   / ��  ��  ��  ��  ��   �  � � � l     ��������  ��  ��   �  � � � l      �� � ���   � � �
 * Get the current demo mode.
 *   1) small dashboard with proximity detection
 *   2) large dashboard with proximity detection
 *   3) presentation viewer
     � � � �< 
   *   G e t   t h e   c u r r e n t   d e m o   m o d e . 
   *       1 )   s m a l l   d a s h b o a r d   w i t h   p r o x i m i t y   d e t e c t i o n 
   *       2 )   l a r g e   d a s h b o a r d   w i t h   p r o x i m i t y   d e t e c t i o n 
   *       3 )   p r e s e n t a t i o n   v i e w e r 
   �  � � � l  � � ����� � Q   � � � � � � r   � � � � � I  � ��� ���
�� .sysoexecTEXT���     TEXT � m   � � � � � � � \ d e f a u l t s   r e a d   c o m . a p l i x . R e m o t e D i s p l a y   d e m o M o d e��   � o      ���� 0 demomode demoMode � R      ������
�� .ascrerr ****      � ****��  ��   � r   � � � � � m   � � � � � � �  1 � o      ���� 0 demomode demoMode��  ��   �  � � � l     ��������  ��  ��   �  � � � l      �� � ���   � 4 .
 * Get the current IP address of the phone.
     � � � � \ 
   *   G e t   t h e   c u r r e n t   I P   a d d r e s s   o f   t h e   p h o n e . 
   �  � � � l  � � ���~ � Q   � � � � � � r   � � � � � I  � ��} ��|
�} .sysoexecTEXT���     TEXT � m   � � � � � � � X d e f a u l t s   r e a d   c o m . a p l i x . R e m o t e D i s p l a y   i p a d d r�|   � o      �{�{ 
0 ipaddr   � R      �z�y�x
�z .ascrerr ****      � ****�y  �x   � r   � � � � � m   � � � � � � �   � o      �w�w 
0 ipaddr  �  �~   �  � � � l     �v�u�t�v  �u  �t   �  � � � l      �s � ��s   � ; 5
 * Get the current Bluetooth address of the phone.
     � � � � j 
   *   G e t   t h e   c u r r e n t   B l u e t o o t h   a d d r e s s   o f   t h e   p h o n e . 
   �  � � � l  � ��r�q � Q   � � � � � r   � �   I  � ��p�o
�p .sysoexecTEXT���     TEXT m   � � � X d e f a u l t s   r e a d   c o m . a p l i x . R e m o t e D i s p l a y   b t a d d r�o   o      �n�n 
0 btaddr   � R      �m�l�k
�m .ascrerr ****      � ****�l  �k   � r    m    �   o      �j�j 
0 btaddr  �r  �q   � 	
	 l     �i�h�g�i  �h  �g  
  l      �f�f   : 4
 * Confirm current demo mode or obtain a new one.
     � h 
   *   C o n f i r m   c u r r e n t   d e m o   m o d e   o r   o b t a i n   a   n e w   o n e . 
    l )�e�d I )�c
�c .sysodlogaskr        TEXT m   � " C h o o s e   d e m o   m o d e : �b
�b 
btns J    m   �  S m a l l   D a s h b o a r d  m     �!!  L a r g e   D a s h b o a r d "�a" m  ## �$$  P r e s e n t a t i o n�a   �`%�_
�` 
dflt% I #�^&�]
�^ .sysorondlong        doub& o  �\�\ 0 demomode demoMode�]  �_  �e  �d   '(' l *5)�[�Z) r  *5*+* n  *1,-, 1  -1�Y
�Y 
bhit- l *-.�X�W. 1  *-�V
�V 
rslt�X  �W  + o      �U�U 0 demomode demoMode�[  �Z  ( /0/ l 6e1�T�S1 Z  6e23452 = 6=676 o  69�R�R 0 demomode demoMode7 m  9<88 �99  S m a l l   D a s h b o a r d3 r  @G:;: m  @C<< �==  1; o      �Q�Q 0 demomode demoMode4 >?> = JQ@A@ o  JM�P�P 0 demomode demoModeA m  MPBB �CC  L a r g e   D a s h b o a r d? D�OD r  T[EFE m  TWGG �HH  2F o      �N�N 0 demomode demoMode�O  5 r  ^eIJI m  ^aKK �LL  3J o      �M�M 0 demomode demoMode�T  �S  0 MNM l     �L�K�J�L  �K  �J  N OPO l      �IQR�I  Q B <
 * Ask if we should try to set up an ad-hoc WiFi network.
    R �SS x 
   *   A s k   i f   w e   s h o u l d   t r y   t o   s e t   u p   a n   a d - h o c   W i F i   n e t w o r k . 
  P TUT l f�V�H�GV I f��FWX
�F .sysodlogaskr        TEXTW m  fiYY �ZZ 6 U s e   a n   a d - h o c   W i F i   n e t w o r k ?X �E[\
�E 
btns[ J  lw]] ^_^ m  lo`` �aa  C a n c e l_ bcb m  ordd �ee  Y e sc f�Df m  rugg �hh  N o�D  \ �Ci�B
�C 
dflti m  z{�A�A �B  �H  �G  U jkj l ��l�@�?l r  ��mnm n  ��opo 1  ���>
�> 
bhitp l ��q�=�<q 1  ���;
�; 
rslt�=  �<  n o      �:�: 0 useadhoc useAdhoc�@  �?  k rsr l     �9�8�7�9  �8  �7  s tut l ��v�6�5v Z  ��wx�4�3w = ��yzy o  ���2�2 0 useadhoc useAdhocz m  ��{{ �||  Y e sx k  ��}} ~~ l  ���1���1  � � �
   * Use the computer name for the ad-hoc network name.  If it's more than 30
   * characters the GUI scripting gets confused, so truncate it.
      � ���& 
       *   U s e   t h e   c o m p u t e r   n a m e   f o r   t h e   a d - h o c   n e t w o r k   n a m e .     I f   i t ' s   m o r e   t h a n   3 0 
       *   c h a r a c t e r s   t h e   G U I   s c r i p t i n g   g e t s   c o n f u s e d ,   s o   t r u n c a t e   i t . 
       ��� r  ����� m  ���0
�0 boovfals� o      �/�/ 0 networkexists networkExists� ��� r  ����� n  ����� 1  ���.
�. 
sicn� l ����-�,� I ���+�*�)
�+ .sysosigtsirr   ��� null�*  �)  �-  �,  � o      �(�( 0 networkname networkName� ��� Z  �����'�&� ?  ����� l ����%�$� I ���#��"
�# .corecnte****       ****� n ����� 2 ���!
�! 
cha � o  ��� �  0 networkname networkName�"  �%  �$  � m  ���� � r  ����� c  ����� n  ����� 7 �����
� 
cha � m  ���� � m  ���� � o  ���� 0 networkname networkName� m  ���
� 
TEXT� o      �� 0 networkname networkName�'  �&  � ��� l ������  �  �  � ��� l  ������  � � �
   * Check for for existing ad-hoc mode.  Note that grep will return an error
   * status of 1 if it doesn't find a match, so we have to use a try/on error
   * block.
      � ���X 
       *   C h e c k   f o r   f o r   e x i s t i n g   a d - h o c   m o d e .     N o t e   t h a t   g r e p   w i l l   r e t u r n   a n   e r r o r 
       *   s t a t u s   o f   1   i f   i t   d o e s n ' t   f i n d   a   m a t c h ,   s o   w e   h a v e   t o   u s e   a   t r y / o n   e r r o r 
       *   b l o c k . 
      � ��� Q  ������ r  ����� I �����
� .sysoexecTEXT���     TEXT� m  ���� ��� � / S y s t e m / L i b r a r y / P r i v a t e F r a m e w o r k s / A p p l e 8 0 2 1 1 . f r a m e w o r k / V e r s i o n s / C u r r e n t / R e s o u r c e s / a i r p o r t   - I   |   g r e p   - c   I B S S�  � o      �� 0 adhocexists adhocExists� R      ���
� .ascrerr ****      � ****�  �  � r  ����� m  ���� ���  0� o      �� 0 adhocexists adhocExists� ��� l ������  �  �  � ��� l  ���
���
  �2,
   * Turn on Airport if necessary and set up ad-hoc WiFi.  This is done through
   * GUI scripting and is probably fairly fragile.  Apparently "airport -i" used
   * to allow setting up a network in IBSS mode (ad-hoc), but that option no
   * longer seems to be available, so we have to script through the GUI.
   * 
   * Check if the ad-hoc network already exists even if we know we're in managed
   * mode and possibly associated with an access point already.  This happens if
   * we set up an ad-hoc network previously but then went into managed mode
   * while the phone was still associated with the Mac; normally, the ad-hoc
   * network is removed when going into managed mode or turning off Airport.  If
   * we try to create a network that already exists then it becomes difficult to
   * recover.
      � ���X 
       *   T u r n   o n   A i r p o r t   i f   n e c e s s a r y   a n d   s e t   u p   a d - h o c   W i F i .     T h i s   i s   d o n e   t h r o u g h 
       *   G U I   s c r i p t i n g   a n d   i s   p r o b a b l y   f a i r l y   f r a g i l e .     A p p a r e n t l y   " a i r p o r t   - i "   u s e d 
       *   t o   a l l o w   s e t t i n g   u p   a   n e t w o r k   i n   I B S S   m o d e   ( a d - h o c ) ,   b u t   t h a t   o p t i o n   n o 
       *   l o n g e r   s e e m s   t o   b e   a v a i l a b l e ,   s o   w e   h a v e   t o   s c r i p t   t h r o u g h   t h e   G U I . 
       *   
       *   C h e c k   i f   t h e   a d - h o c   n e t w o r k   a l r e a d y   e x i s t s   e v e n   i f   w e   k n o w   w e ' r e   i n   m a n a g e d 
       *   m o d e   a n d   p o s s i b l y   a s s o c i a t e d   w i t h   a n   a c c e s s   p o i n t   a l r e a d y .     T h i s   h a p p e n s   i f 
       *   w e   s e t   u p   a n   a d - h o c   n e t w o r k   p r e v i o u s l y   b u t   t h e n   w e n t   i n t o   m a n a g e d   m o d e 
       *   w h i l e   t h e   p h o n e   w a s   s t i l l   a s s o c i a t e d   w i t h   t h e   M a c ;   n o r m a l l y ,   t h e   a d - h o c 
       *   n e t w o r k   i s   r e m o v e d   w h e n   g o i n g   i n t o   m a n a g e d   m o d e   o r   t u r n i n g   o f f   A i r p o r t .     I f 
       *   w e   t r y   t o   c r e a t e   a   n e t w o r k   t h a t   a l r e a d y   e x i s t s   t h e n   i t   b e c o m e s   d i f f i c u l t   t o 
       *   r e c o v e r . 
      � ��	� Z  ������� = ���� o  ���� 0 adhocexists adhocExists� m  � �� ���  0� O  ���� O  
���� k  ��� ��� O  ��� k  �� ��� l ����  �   Find Airport Menu.   � ��� &   F i n d   A i r p o r t   M e n u .� ��� r  2��� n  .��� 1  *.�
� 
valL� n  *��� 4  #*��
� 
attr� m  &)�� ���  A X D e s c r i p t i o n� 2 #�
� 
mbri� o      �� 0 menu_extras  � ��� Y  3\�� ����� Z CW������� = CO��� n  CK��� 4  FK���
�� 
cobj� o  IJ���� 0 the_menu  � o  CF���� 0 menu_extras  � m  KN�� ��� $ A i r p o r t   M e n u   E x t r a�  S  RS��  ��  �  0 the_menu  � m  67���� � l 7>������ I 7>�����
�� .corecnte****       ****� o  7:���� 0 menu_extras  ��  ��  ��  ��  � ��� l ]]��������  ��  ��  � ���� O  ]��� k  f�� ��� I fr�����
�� .prcsperfactT       actT� 4  fn���
�� 
actT� m  jm�� ���  A X P r e s s��  � ��� l ss������  � %  If Airport is off, turn it on.   � ��� >   I f   A i r p o r t   i s   o f f ,   t u r n   i t   o n .� ��� Z  s� ����  = s� n  s� 1  ~���
�� 
titl n  s~ 4  y~��
�� 
menI m  |}����  4  sy��	
�� 
menE	 m  wx����  m  ��

 �  T u r n   A i r p o r t   O n k  ��  I ������
�� .prcsperfactT       actT n  �� 4  ����
�� 
actT m  �� �  A X P r e s s n  �� 4  ����
�� 
menI m  �� �  T u r n   A i r p o r t   O n 4  ����
�� 
menE m  ������ ��   �� I ������
�� .prcsperfactT       actT 4  ����
�� 
actT m  �� �  A X P r e s s��  ��  ��  ��  �  !  l ����������  ��  ��  ! "#" l ����$%��  $ = 7 Check if an ad-hoc network already exists in the list.   % �&& n   C h e c k   i f   a n   a d - h o c   n e t w o r k   a l r e a d y   e x i s t s   i n   t h e   l i s t .# '(' Y  ��)��*+��) Z  ��,-����, = ��./. n  ��010 1  ����
�� 
titl1 n  ��232 4  ����4
�� 
menI4 o  ������ 0 network_item  3 4  ����5
�� 
menE5 m  ������ / o  ������ 0 networkname networkName- k  ��66 787 r  ��9:9 m  ����
�� boovtrue: o      ���� 0 networkexists networkExists8 ;��;  S  ����  ��  ��  �� 0 network_item  * m  ������ + l ��<����< I ����=��
�� .corecnte****       ****= n  ��>?> m  ����
�� 
menI? 4  ����@
�� 
menE@ m  ������ ��  ��  ��  ��  ( A��A Z  �BC��DB = ��EFE o  ������ 0 networkexists networkExistsF m  ����
�� boovtrueC k  ��GG HIH l ����JK��  J @ : Originally dismissed the wifi menu since it doesn't fully   K �LL t   O r i g i n a l l y   d i s m i s s e d   t h e   w i f i   m e n u   s i n c e   i t   d o e s n ' t   f u l l yI MNM l ����OP��  O > 8 update to show the new association, but for some reason   P �QQ p   u p d a t e   t o   s h o w   t h e   n e w   a s s o c i a t i o n ,   b u t   f o r   s o m e   r e a s o nN RSR l ����TU��  T 8 2 reassociation happens much faster if it stays up.   U �VV d   r e a s s o c i a t i o n   h a p p e n s   m u c h   f a s t e r   i f   i t   s t a y s   u p .S W��W l ����XY��  X   perform action "AXPress"   Y �ZZ 2   p e r f o r m   a c t i o n   " A X P r e s s "��  ��  D k  �[[ \]\ l ����^_��  ^ - ' Bring up the Create Network... dialog.   _ �`` N   B r i n g   u p   t h e   C r e a t e   N e t w o r k . . .   d i a l o g .] a��a I ���b��
�� .prcsperfactT       actTb n  �cdc 4  
��e
�� 
actTe m  ff �gg  A X P r e s sd n  �
hih 4  
��j
�� 
menIj m  	kk �ll  C r e a t e   N e t w o r k &i 4  ���m
�� 
menEm m  ���� ��  ��  ��  � 4  ]c��n
�� 
mbrin o  ab���� 0 the_menu  ��  � 4  ��o
�� 
mbaro m  ���� � pqp l ��������  ��  ��  q r��r Z  �st��us = vwv o  ���� 0 networkexists networkExistsw m  ��
�� boovtruet k   Vxx yzy l   ��{|��  { 5 / Reasssociate with the existing ad-hoc network.   | �}} ^   R e a s s s o c i a t e   w i t h   t h e   e x i s t i n g   a d - h o c   n e t w o r k .z ~~ I  @����
�� .sysodlogaskr        TEXT� b   +��� b   '��� m   #�� ��� � T h e   d e m o   n e e d s   t o   r e - a s s o c i a t e   w i t h   t h e   e x i s t i n g   a d - h o c   n e t w o r k   "� o  #&���� 0 networkname networkName� m  '*�� ��� � "   b u t   n e e d s   a d m i n i s t r a t o r   p r i v i l e g e s   t o   d o   s o .     P r e s s   C o n t i n u e   t o   e n t e r   y o u r   p a s s w o r d   o r   C a n c e l   i n s t e a d .� ����
�� 
btns� J  .6�� ��� m  .1�� ���  C a n c e l� ���� m  14�� ���  C o n t i n u e��  � �����
�� 
dflt� m  9:���� ��   ��� l AA��������  ��  ��  � ���� I AV����
�� .sysoexecTEXT���     TEXT� b  AN��� b  AL��� b  AH��� m  AD�� ��� d / u s r / s b i n / n e t w o r k s e t u p   - s e t a i r p o r t n e t w o r k   A i r P o r t  � o  DG���� 0 networkname networkName� m  HK�� ���   � o  LM���� 
0 wepkey  � �����
�� 
badm� m  QR��
�� boovtrue��  ��  ��  u k  Y��� ��� l YY������  � 3 - Enter information into Create Network Dialog   � ��� Z   E n t e r   i n f o r m a t i o n   i n t o   C r e a t e   N e t w o r k   D i a l o g� ���� O  Y���� k  b��� ��� l bb������  �   Require Password   � ��� "   R e q u i r e   P a s s w o r d� ��� I bl�����
�� .prcsclicuiel    ��� uiel� 4  bh���
�� 
chbx� m  fg���� ��  � ��� l mm��������  ��  ��  � ��� l mm������  �   Fill in information   � ��� (   F i l l   i n   i n f o r m a t i o n� ��� r  my��� o  mn���� 
0 wepkey  � n      ��� 1  tx��
�� 
valL� 4  nt���
�� 
txtf� m  rs���� � ��� r  z���� o  z{���� 
0 wepkey  � n      ��� 1  ����
�� 
valL� 4  {����
�� 
txtf� m  ����� � ��� l ������~��  �  �~  � ��� l ���}���}  � 1 + Set name last so OK button will be enabled   � ��� V   S e t   n a m e   l a s t   s o   O K   b u t t o n   w i l l   b e   e n a b l e d� ��� r  ����� o  ���|�| 0 networkname networkName� n      ��� 1  ���{
�{ 
valL� 4  ���z�
�z 
txtf� m  ���y�y � ��� l ���x�w�v�x  �w  �v  � ��� l ���u���u  �  	 Click OK   � ���    C l i c k   O K� ��t� I ���s��r
�s .prcsclicuiel    ��� uiel� 4  ���q�
�q 
butT� m  ���p�p �r  �t  � 4  Y_�o�
�o 
cwin� m  ]^�n�n ��  ��  � 4  
�m�
�m 
prcs� m  �� ���  S y s t e m U I S e r v e r� m  ���                                                                                  sevs  alis    �  
Grendel HD                 Ǟ6QH+     �System Events.app                                               *��7��        ����  	                CoreServices    Ǟ��      �8'7       �   H   G  8Grendel HD:System:Library:CoreServices:System Events.app  $  S y s t e m   E v e n t s . a p p   
 G r e n d e l   H D  -System/Library/CoreServices/System Events.app   / ��  �  �  �	  �4  �3  �6  �5  u ��� l     �l�k�j�l  �k  �j  � ��� l      �i���i  � 3 -
 * Confirm IP address or obtain a new one.
    � ��� Z 
   *   C o n f i r m   I P   a d d r e s s   o r   o b t a i n   a   n e w   o n e . 
  � ��� l �C��h�g� Z  �C���f�� = ����� o  ���e�e 0 useadhoc useAdhoc� m  ���� ���  Y e s� I �"�d��
�d .sysodlogaskr        TEXT� b  ���� b  �� � b  �� b  �� b  �� b  �� b  ��	
	 b  �� b  �� b  �� b  �� b  �� b  �� b  �� b  �� b  �� b  �� b  ��  b  ��!"! b  ��#$# m  ��%% �&& L A i r p o r t   i s   i n   n o w   i n   a d - h o c   W i F i   m o d e .$ o  ���c
�c 
ret " o  ���b
�b 
ret   m  ��'' �((  N e t w o r k   n a m e :   o  ���a�a 0 networkname networkName o  ���`
�` 
ret  m  ��)) �** , E n c r y p t i o n :   4 0 - b i t   W E P o  ���_
�_ 
ret  m  ��++ �,,  p a s s w o r d :     o  ���^�^ 
0 wepkey   o  ���]
�] 
ret  o  ���\
�\ 
ret  m  ��-- �.. ^ E n s u r e   t h a t   t h e   p h o n e   i s   c o n n e c t e d   t o   n e t w o r k   " o  ���[�[ 0 networkname networkName
 m  ��// �00  "   a n d   e n t e r   o r   c o n f i r m   t h e   p h o n e ' s   I P   a d d r e s s   h e r e .     T h e   p h o n e ' s   I P   a d d r e s s   f o r   a d - h o c   n e t w o r k s   i s   l i n k - l o c a l   ( 1 6 9 . 2 5 4 . x x x . y y y ) . o  ���Z
�Z 
ret  o  ���Y
�Y 
ret  m  ��11 �22� I f   y o u   h a v e   p r o b l e m s   c o n n e c t i n g ,   t r y   r e m o v i n g   a n y   c u r r e n t   n e t w o r k   i n f o r m a t i o n   f r o m   t h e   p h o n e   a n d   r e - e n t e r i n g   t h e   a b o v e   n e t w o r k   i n f o r m a t i o n .     T u r n i n g   o f f   a l l   B l u e t o o t h   d e v i c e s   c a n   h e l p   a s   w e l l . o  ���X
�X 
ret   o  ��W
�W 
ret � m  33 �44 d O t h e r w i s e ,   c o n n e c t   t o   a   m a n a g e d   W i F i   a c c e s s   p o i n t .� �V56
�V 
dtxt5 o  
�U�U 
0 ipaddr  6 �T78
�T 
btns7 J  99 :;: m  << �==  C a n c e l; >�S> m  ?? �@@  C o n t i n u e�S  8 �RA�Q
�R 
dfltA m  �P�P �Q  �f  � I %C�OBC
�O .sysodlogaskr        TEXTB m  %(DD �EE Z E n t e r   o r   c o n f i r m   t h e   p h o n e ' s   I P   a d d r e s s   h e r e .C �NFG
�N 
dtxtF o  +.�M�M 
0 ipaddr  G �LHI
�L 
btnsH J  19JJ KLK m  14MM �NN  C a n c e lL O�KO m  47PP �QQ  C o n t i n u e�K  I �JR�I
�J 
dfltR m  <=�H�H �I  �h  �g  � STS l DOU�G�FU r  DOVWV n  DKXYX 1  GK�E
�E 
ttxtY l DGZ�D�CZ 1  DG�B
�B 
rslt�D  �C  W o      �A�A 
0 ipaddr  �G  �F  T [\[ l     �@�?�>�@  �?  �>  \ ]^] l P,_�=�<_ Z  P,`a�;b` G  Pecdc = PWefe o  PS�:�: 0 demomode demoModef m  SVgg �hh  1d = Zaiji o  Z]�9�9 0 demomode demoModej m  ]`kk �ll  2a k  h"mm non l  hh�8pq�8  p > 8
   * Confirm bluetooth address or obtain a new one.
      q �rr p 
       *   C o n f i r m   b l u e t o o t h   a d d r e s s   o r   o b t a i n   a   n e w   o n e . 
      o sts I h��7uv
�7 .sysodlogaskr        TEXTu b  hwwxw b  hsyzy b  ho{|{ m  hk}} �~~ � E n t e r   o r   c o n f i r m   t h e   B l u e t o o t h   h a r d w a r e   a d d r e s s   o f   t h e   p h o n e .   U s e   a   f o r m a t   l i k e   t h i s :| o  kn�6
�6 
ret z o  or�5
�5 
ret x m  sv ��� " 0 0 - 1 1 - 2 2 - 3 3 - 4 4 - 5 5v �4��
�4 
dtxt� o  z}�3�3 
0 btaddr  � �2��
�2 
btns� J  ���� ��� m  ���� ���  C a n c e l� ��1� m  ���� ���  C o n t i n u e�1  � �0��/
�0 
dflt� m  ���.�. �/  t ��� r  ����� n  ����� 1  ���-
�- 
ttxt� l ����,�+� 1  ���*
�* 
rslt�,  �+  � o      �)�) 
0 btaddr  � ��� I ���(��'
�( .sysoexecTEXT���     TEXT� b  ����� m  ���� ��� \ d e f a u l t s   w r i t e   c o m . a p l i x . R e m o t e D i s p l a y   b t a d d r  � o  ���&�& 
0 btaddr  �'  � ��� l ���%�$�#�%  �$  �#  � ��� l  ���"���"  �_Y
   * Set up Proximity.app.  The app has been hacked to accept the Bluetooth
   * device address directly and to allow setting of the page timeout.  There
   * are 1600 slots in 1 second; the default is 8192 (5.12 seconds), so here
   * we set the timeout to a little longer than 1 second to make the proximity
   * detection less sensitive.
      � ���� 
       *   S e t   u p   P r o x i m i t y . a p p .     T h e   a p p   h a s   b e e n   h a c k e d   t o   a c c e p t   t h e   B l u e t o o t h 
       *   d e v i c e   a d d r e s s   d i r e c t l y   a n d   t o   a l l o w   s e t t i n g   o f   t h e   p a g e   t i m e o u t .     T h e r e 
       *   a r e   1 6 0 0   s l o t s   i n   1   s e c o n d ;   t h e   d e f a u l t   i s   8 1 9 2   ( 5 . 1 2   s e c o n d s ) ,   s o   h e r e 
       *   w e   s e t   t h e   t i m e o u t   t o   a   l i t t l e   l o n g e r   t h a n   1   s e c o n d   t o   m a k e   t h e   p r o x i m i t y 
       *   d e t e c t i o n   l e s s   s e n s i t i v e . 
      � ��� I ���!�� 
�! .sysoexecTEXT���     TEXT� b  ����� m  ���� ��� t d e f a u l t s   w r i t e   c o m . r e d u x c o m p u t i n g . P r o x i m i t y   d e v i c e A d d r e s s  � o  ���� 
0 btaddr  �   � ��� I �����
� .sysoexecTEXT���     TEXT� m  ���� ��� � d e f a u l t s   w r i t e   c o m . r e d u x c o m p u t i n g . P r o x i m i t y   p a g e T i m e o u t S l o t s   1 7 0 0�  � ��� I �����
� .sysoexecTEXT���     TEXT� m  ���� ��� v d e f a u l t s   w r i t e   c o m . r e d u x c o m p u t i n g . P r o x i m i t y   t i m e r I n t e r v a l   2�  � ��� I �����
� .sysoexecTEXT���     TEXT� b  ����� b  ����� m  ���� ��� | d e f a u l t s   w r i t e   c o m . r e d u x c o m p u t i n g . P r o x i m i t y   i n R a n g e S c r i p t P a t h  � o  ���� 0 cachedir  � m  ���� ��� $ / s t a r t - r e m o t e . s c p t�  � ��� I �����
� .sysoexecTEXT���     TEXT� b  ����� b  ����� m  ���� ��� � d e f a u l t s   w r i t e   c o m . r e d u x c o m p u t i n g . P r o x i m i t y   o u t O f R a n g e S c r i p t P a t h  � o  ���� 0 cachedir  � m  ���� ��� " / k i l l - r e m o t e . s c p t�  � ��� l ������  �  �  � ��� l  ������  � 3 -
   * Ask if we should run Proximity.app.
      � ��� Z 
       *   A s k   i f   w e   s h o u l d   r u n   P r o x i m i t y . a p p . 
      � ��� I ����
� .sysodlogaskr        TEXT� b  ����� b  ����� b  ����� b  ����� b  ����� b  ����� m  ���� ��� n D o   y o u   w a n t   t o   r u n   P r o x i m i t y . a p p   t o   s t a r t   t h e   d e m o   n o w ?� o  ���
� 
ret � o  ���
� 
ret � m  ���� ��� I f   t h e   p h o n e   i s   n o t   p o w e r e d   o n   w i t h   B l u e t o o t h   e n a b l e d   t h e n   P r o x i m i t y . a p p   w i l l   b e c o m e   u n r e s p o n s i v e ,   s o   m a k e   s u r e   t h e   p h o n e   i s   r e a d y .� o  ���
� 
ret � o  ���
� 
ret � m  ���� ��� � I f   t h e   p h o n e   i s   r e a d y   a n d   i n   r a n g e   t h e n   P r o x i m i t y . a p p   w i l l   l a u n c h   t h e   r e m o t e   d i s p l a y   a   f e w   s e c o n d s   a f t e r   s e l e c t i n g   " Y e s . "� ���
� 
btns� J  �� ��� m  �� ���  C a n c e l� ��� m  �� ���  N o� ��
� m  
�� ���  Y e s�
  � �	��
�	 
dflt� m  �� �  � ��� r  "��� n  ��� 1  �
� 
bhit� l ���� 1  �
� 
rslt�  �  � o      �� 0 runproximity runProximity�  �;  b r  %,��� m  %(�� ���  N o� o      � �  0 runproximity runProximity�=  �<  ^    l     ��������  ��  ��    l      ����  �|
 * Set up remote-jblend.app.  Note that we don't set the bg (background)
 * arg to 1, so the remote display does NOT fork into the background.  The
 * fork() is necessary in order to keep BlueProximity on Linux from blocking,
 * but Proximity.app on Mac doesn't need it and in fact it provokes bugs that
 * cause the remote display to launch with two instances in some cases.  
     �� 
   *   S e t   u p   r e m o t e - j b l e n d . a p p .     N o t e   t h a t   w e   d o n ' t   s e t   t h e   b g   ( b a c k g r o u n d ) 
   *   a r g   t o   1 ,   s o   t h e   r e m o t e   d i s p l a y   d o e s   N O T   f o r k   i n t o   t h e   b a c k g r o u n d .     T h e 
   *   f o r k ( )   i s   n e c e s s a r y   i n   o r d e r   t o   k e e p   B l u e P r o x i m i t y   o n   L i n u x   f r o m   b l o c k i n g , 
   *   b u t   P r o x i m i t y . a p p   o n   M a c   d o e s n ' t   n e e d   i t   a n d   i n   f a c t   i t   p r o v o k e s   b u g s   t h a t 
   *   c a u s e   t h e   r e m o t e   d i s p l a y   t o   l a u n c h   w i t h   t w o   i n s t a n c e s   i n   s o m e   c a s e s .     
    l -8	����	 I -8��
��
�� .sysoexecTEXT���     TEXT
 b  -4 m  -0 � ` d e f a u l t s   w r i t e   c o m . a p l i x . R e m o t e D i s p l a y   d e m o M o d e   o  03���� 0 demomode demoMode��  ��  ��    l 9D���� I 9D����
�� .sysoexecTEXT���     TEXT b  9@ m  9< � \ d e f a u l t s   w r i t e   c o m . a p l i x . R e m o t e D i s p l a y   i p a d d r   o  <?���� 
0 ipaddr  ��  ��  ��    l EL���� I EL����
�� .sysoexecTEXT���     TEXT m  EH � ` d e f a u l t s   w r i t e   c o m . a p l i x . R e m o t e D i s p l a y   p o r t   9 9 9 9��  ��  ��    l     ��������  ��  ��     l M�!����! Z  M�"#��$" = MT%&% o  MP���� 0 runproximity runProximity& m  PS'' �((  Y e s# O Wc)*) I ]b������
�� .aevtoappnull  �   � ****��  ��  * m  WZ++�                                                                                      @ alis    �  
Grendel HD                 Ǟ6QH+   /��Proximity.app                                                   /�n�8�        ����  	                Darwin    Ǟ��      ��1     /�� /̶ /̳ O�  GGrendel HD:export:MWC-2010-multidisplay-demo:build:Darwin:Proximity.app     P r o x i m i t y . a p p   
 G r e n d e l   H D  <export/MWC-2010-multidisplay-demo/build/Darwin/Proximity.app  / ��  ��  $ k  f�,, -.- I f���/0
�� .sysodlogaskr        TEXT/ m  fi11 �22 x D o   y o u   w a n t   t o   r u n   t h e   r e m o t e   d i s p l a y   t o   s t a r t   t h e   d e m o   n o w ?0 ��34
�� 
btns3 J  lw55 676 m  lo88 �99  C a n c e l7 :;: m  or<< �==  N o; >��> m  ru?? �@@  Y e s��  4 ��A��
�� 
dfltA m  z{���� ��  . BCB r  ��DED n  ��FGF 1  ����
�� 
bhitG l ��H����H 1  ����
�� 
rslt��  ��  E o      ���� 0 rundemo runDemoC I��I Z  ��JK����J = ��LML o  ������ 0 rundemo runDemoM m  ��NN �OO  Y e sK O ��PQP I ��������
�� .miscactvnull��� ��� null��  ��  Q m  ��RR
                                                                                      @ alis    �  
Grendel HD                 Ǟ6QH+   /��remote-jblend.app                                               /���8�        ����  	                Darwin    Ǟ��      ��     /�� /̶ /̳ O�  KGrendel HD:export:MWC-2010-multidisplay-demo:build:Darwin:remote-jblend.app   $  r e m o t e - j b l e n d . a p p   
 G r e n d e l   H D  @export/MWC-2010-multidisplay-demo/build/Darwin/remote-jblend.app  / ��  ��  ��  ��  ��  ��    S��S l     ��������  ��  ��  ��       ��TUV��  T ������ 0 killapp killApp
�� .aevtoappnull  �   � ****U �� f����WX���� 0 killapp killApp�� ��Y�� Y  ���� 0 appname appName��  W ���� 0 appname appNameX  u w��
�� .sysoexecTEXT���     TEXT�� 
�%�%j V ��Z����[\��
�� .aevtoappnull  �   � ****Z k    �]]  ^^  __  !``  +aa  @bb  Ncc  �dd  �ee  �ff  �gg  �hh  �ii  �jj kk 'll /mm Tnn joo tpp �qq Srr ]ss tt uu vv ����  ��  ��  [ ������ 0 the_menu  �� 0 network_item  \ � �� �� '�������� <������ J L X Z ��� � ����� ����� ����� ��� ����� ������� ������� � ��� ��� #��������8<BGKY`dg��{�������������������������������������������������
kf�������������������%')+-/13��<?DMP�gk�~}�����������������}�'+�|18<?�{NR�� 
0 wepkey  �� 0 cachedir  
�� .sysoexecTEXT���     TEXT
�� 
rtyp
�� 
ctxt
�� .earsffdralis        afdr
�� 
psxp
�� 
strq�� 0 	scriptdir  �� 0 killapp killApp
�� 
uien�� 0 
ui_enabled 
UI_enabled
�� .miscactvnull��� ��� null
�� 
xppb
�� kfrmID  
�� 
xpcp
�� 
ret 
�� 
disp
�� 
btns
�� 
dflt�� 
�� .sysodlogaskr        TEXT�� 0 demomode demoMode��  ��  �� 
0 ipaddr  �� 
0 btaddr  
�� .sysorondlong        doub�� 
�� 
rslt
�� 
bhit�� 0 useadhoc useAdhoc�� 0 networkexists networkExists
�� .sysosigtsirr   ��� null
�� 
sicn�� 0 networkname networkName
�� 
cha 
�� .corecnte****       ****�� 
�� 
TEXT�� 0 adhocexists adhocExists
�� 
prcs
�� 
mbar
�� 
mbri
�� 
attr
�� 
valL�� 0 menu_extras  
�� 
cobj
�� 
actT
�� .prcsperfactT       actT
�� 
menE
�� 
menI
�� 
titl
�� 
badm
�� 
cwin
�� 
chbx
�� .prcsclicuiel    ��� uiel
�� 
txtf
�� 
butT
�� 
dtxt
� 
ttxt
�~ 
bool�} 0 runproximity runProximity
�| .aevtoappnull  �   � ****�{ 0 rundemo runDemo����E�O�E�O��%j O)��l �%�,�,E�O��%�%�%j O��%a %�%j O*a k+ O*a k+ Oa  *a ,E` UO_ f  Ia  ?*j O*a a a 0*a ,FOa _ %_ %a %a  ka !a "kva #ka $ %UY hO a &j E` 'W X ( )a *E` 'O a +j E` ,W X ( )a -E` ,O a .j E` /W X ( )a 0E` /Oa 1a !a 2a 3a 4mva #_ 'j 5a 6 %O_ 7a 8,E` 'O_ 'a 9  a :E` 'Y _ 'a ;  a <E` 'Y 	a =E` 'Oa >a !a ?a @a Amva #ma 6 %O_ 7a 8,E` BO_ Ba C fE` DO*j Ea F,E` GO_ Ga H-j Ia J _ G[a H\[Zk\Za J2a K&E` GY hO a Lj E` MW X ( )a NE` MO_ Ma O �a �*a Pa Q/�*a Rk/ �*a S-a Ta U/a V,E` WO (k_ Wj Ikh  _ Wa X�/a Y  Y h[OY��O*a S�/ �*a Za [/j \O*a ]k/a ^l/a _,a `  **a ]k/a ^a a/a Za b/j \O*a Za c/j \Y hO <m*a ]k/a ^,j Ikh *a ]k/a ^�/a _,_ G  eE` DOY h[OY��O_ De  hY *a ]k/a ^a d/a Za e/j \UUO_ De  ;a f_ G%a g%a !a ha ilva #la 6 %Oa j_ G%a k%�%a lel Y J*a mk/ @*a nk/j oO�*a pl/a V,FO�*a pm/a V,FO_ G*a pk/a V,FO*a qk/j oUUUY hY hO_ Ba r  qa s_ %_ %a t%_ G%_ %a u%_ %a v%�%_ %_ %a w%_ G%a x%_ %_ %a y%_ %_ %a z%a {_ ,a !a |a }lva #la $ %Y  a ~a {_ ,a !a a �lva #la $ %O_ 7a �,E` ,O_ 'a � 
 _ 'a � a �& �a �_ %_ %a �%a {_ /a !a �a �lva #la $ %O_ 7a �,E` /Oa �_ /%j Oa �_ /%j Oa �j Oa �j Oa ��%a �%j Oa ��%a �%j Oa �_ %_ %a �%_ %_ %a �%a !a �a �a �mva #ma 6 %O_ 7a 8,E` �Y 	a �E` �Oa �_ '%j Oa �_ ,%j Oa �j O_ �a �  a � *j �UY Da �a !a �a �a �mva #ma 6 %O_ 7a 8,E` �O_ �a �  a � *j UY hascr  ��ޭ