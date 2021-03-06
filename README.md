# CSET
  
개발 중단. llvm으로 재작성할 예정. 
    
--사용법--

![Alt text](C:/Users/comeng-PC/Documents/Bandicam/path.jpg)
cset.exe 파일이 위치한 경로를 환경변수 PATH에 설정한다.

![Alt text](C:/Users/comeng-PC/Documents/Bandicam/cset.jpg)
그리고 cmd에서 cset을 쳐서 이렇게 드면 정상이다.

그럼 헬로월드를 돌려보자
<pre><code>
import console //콘솔 모듈 사용

func main(args : string[])
{
	console::putline("hello") //콘솔 출력
	console::pause() //콘솔 대기
}
</code></pre>

![Alt text](C:/Users/comeng-PC/Documents/Bandicam/hello.jpg)

--사용법--


확장자 .cset(아직 미적용)


아직 틀만 있음.


명령행인자로 파일들을 전달하면 -...는 옵션. 그 이외에는 소스파일로 인식.
인터프리트하고 하나의 cpp 코드로 뭉쳐서 msvc로 컴파일하는 것까지만 구현.


msvc 내장.


아직까지는 윈도우 종속적임.





-----문법-----


세미콜론 생략가능.

명시도 가능.(코틀린 스타일)

--------------------------------
----------------변수-------------
---------------------------------
가변 변수 선언

mut 변수명 : 타입 = 초기화값; 

mut 변수명 = 초기화값; //타입 추론


불변 변수 선언

const 변수명 : 타입 = 초기화값;

const 변수명 = 초기화값; //타입 추론



컴파일타임 상수 선언

literal 상수명 = 값; //constexpr auto로 변환

-----------------------------------------------


------------------------------------------------
----------------타입---------------------------
------------------------------------------------
정수형
int, uint,
int8, uint8,
int16, uint16,
int32, uint32,
int64, uint64,

byte = unsigned char;

string = std::wstring;

bool

char = wchar_t



레퍼런스타입(C++과 동일)

타입명&


new는 동적할당(unique_ptr로 변환됨)

rcnew도 동적할당(shared_ptr)


----------------------------------------


move 키워드로 등록. (std::move)


함수 선언

func 함수명(파라미터...) -> 리턴타입
{ ... }


리턴타입 생략시 void.


파라미터

(변수명 : 타입, 변수명 : 타입, ...)

디폴트는 const지만. const와 mut로 명시할 수 있음.



----------------------------------------------
-------------------제어구문--------------------
----------------------------------------------
for(;;) 삭제.

for( in ) 유지 (c++'s for( : ))

rfor( in )

while 유지

continue 유지

break; 유지 + 브레이크 레이블 확장

goto 삭제

label 키워드 등록(일단 브레이크 레이블 전용)


switch 유지

case 유지

if 유지

else 유지

------------------------------------------------
ㅤ
<if> -> if ( condition ) ( statement-true | expression-true )
| if ( condition ) ( statement-true | expression-true; ) else ( statement-false | expression-false )

<while> -> while( condition ) ( loop_statement | loop_expression )

<for> -> for( range_declaration in range_expression ) ( loop_statement | loop_expression )

<rfor> -> rfor( range_declaration in range_expression ) ( loop_statement | loop_expression )

<import> -> import module_name

<return> -> return expression(optional)

<break> -> break label(optional)

<continue> -> continue label(optional)

<function> -> func func_name(parameter-list) ->return_type(optional) { body }

<class> -> class class_name inhertance_class(optional) { member_specification }

<lambda> -> $(parameter-list) ->return_type(optional) { body }

------------------------------------------------
---------------------일단 예약--------------------
thread 예약
