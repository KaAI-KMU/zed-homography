# zed-homography
이미지 좌표계 변환 UC-win/Road → ZED (Homography, C++, OpenCV)

<br>

**프로젝트 목적**

이 프로젝트를 하는 이유는 EyeTracker 값들을 통해 UC-win/Road 상에 결과를 출력하는데 이를 카메라 시점에서도 보여주고 싶기 때문이다. 그렇다면 UC-win/Road 상의 시선좌표를 카메라 이미지 상의 UC-win/Road 좌표로 변환시켜줘야한다.

<br>

**Homography Transition**

먼저 실험실 모니터 좌표계와 카메라 이미지 상의 좌표계를 매핑하는 Homography 변환 행렬을 추정. 이후에는 카메라로 촬영한 이미지에서 uc-win/road 상의 특정 점(point)을 찾아 해당 점의 이미지 상 좌표를 얻은 후, 이미 추정된 Homography 변환 행렬을 이용하여 해당 좌표를 카메라 이미지 좌표계 상의 좌표로 변환할 수 있다.

<br>

**필요한 인풋값**

- UC-win/Road 실행화면 (모니터 캡쳐)
- 카메라 이미지 (ZED Camera)
- 모니터 네 모서리의 좌표값 (해상도)
- 카메라 이미지 상의 UC-win/Road 네 모서리 좌표값 (고정되어 있기 때문에 1장만 뽑아서 const로 선언)

<br>

**프로젝트 설계**
- input: Eyetracker 매핑 값을 인풋으로 Homography 프로그램에 넣어줌 (이미 UC-win/Road의 네 모서리 좌표값, 카메라 이미지 상의 네 모서리 좌표값은 const 값으로 선언되어있음)
- output: 카메라 이미지 상의 UC-win/Road에서 gaze point 출력
