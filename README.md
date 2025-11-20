# 🖥️ ObjectPool
Listen Server 환경에서 동작하는 Unreal Engine 5 기반 Object Pool 시스템

![](https://github.com/user-attachments/assets/e5e1ea55-195c-4496-bbe8-d70ed6e38749)

## ⏱️ 개발 기간
- 25.08.04 - 25.08.26

## ⚙️ 개발 환경
- Unreal Engine 5.4.4
- JetBrains Rider 2025.1
- Git

## 👨‍💻 구현 내용
#### Subsystem을 이용한 관리 시스템 구현
- `UWorldSubsystem`을 상속받아 월드 단위에서 자동으로 초기화 및 관리
- Listen Server 환경에 최적화
  - `OnWorldBeginPlay()`를 통해 서버에서만 풀을 초기화
  - 클라이언트 중복 생성 방지
- DataTable을 통한 초기 풀 구성 자동화
  - `ActorClass`, `PoolSize` 등을 로드
  - StreamableManager를 사용해 비동기 로드 처리

#### 타입 기반 풀 구조체(FActorPool)
- `FGameplayTag`를 키로 하는 `TMap`을 사용해 객체 타입별 풀 분리
- 특징
  - 사용 중인 오브젝트가 많아 부족할 경우 자동 확장
  - 풀 확장 시 기존 크기만큼 증가하되, MaxPoolSize 제한 내에서만 생성

#### 컴포넌트와 인터페이스를 활용한 객체 활성화/비활성화
- IPoolableInterface
  - 풀링되는 Actor는 해당 인터페이스를 구현
  - 시스템에서 가져올 때 `OnPoolActivate()`, 반환 시 `OnPoolDeactivate()` 호출
  - 활성/비활성 상태 전환을 명확히 제어 가능
- UPoolableComponent
  - 액터에 부착되어 객체 별 로직을 수행
  - 코드의 중복으로 인한 반복 작업을 피함

## 📊 성능 비교
#### 개요
- 여러 개의 객체를 생성할 때, ObjectPool과 SpawnActor의 성능을 비교

#### 조건
- 넷 모드는 `Play As Listen Server`, 플레어어 수는 2로 세팅 후 게임 실행
- 마우스 우클릭을 유지하여 0.2초마다 100개의 오브젝트를 활성화
- 활성화된 오브젝트는 0.8초 후 비활성화
- 약 10초간 테스트
- 언리얼 인사이트를 통해 결과 확인

#### 결과
- SpawnActor
<img width="678" height="115" alt="SpawnActor 2025-11-20 153538" src="https://github.com/user-attachments/assets/780d5889-d2df-4f7a-916f-4168d702802e" />

- ObjectPool
<img width="649" height="104" alt="ObjectPool 2025-11-20 153548" src="https://github.com/user-attachments/assets/c7e10928-6fda-4b5d-a645-a5e8dc272d05" />

- SpawnActor에 비해 안정적으로 프레임을 유지
