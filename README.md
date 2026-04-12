# Project JA (Action RPG)

> **데이터 기반 Gameplay Ability System** 싱글 플레이어 액션 RPG 프로젝트<br>
> **렌더링 최적화**, **에디터 파이프라인 자동화**를 통한 성능 및 무결성 확보<br>
> 엔진 버전: **Unreal Engine 5.5**

---

## 목차
1. [Overview](#1-overview)
2. [Focus & Goals](#2-focus--goals)
3. [Technical Stack](#3-technical-stack)
4. [Game Preview](#4-game-preview)
5. [Core Implementation](#5-core-implementation)
6. [Troubleshooting](#6-troubleshooting)
7. [Project Structure](#7-project-structure)
8. [Architecture](#8-architecture)
9. [Repository Info](#9-repository-info)
10. [License / Usage](#10-license--usage)
11. [Author](#11-author)

---

## 1. Overview

**Project JA**는 단순 기능 구현을 넘어, 대규모 액션 게임 개발 시 발생할 수 있는 병목과 데이터 오염을 방어하는 시스템 설계 중심의 포트폴리오입니다.
기존 강의의 베이스라인을 기반으로 핵심 로직을 C++로 리팩토링하고, 커스텀 최적화 파이프라인 및 에셋 검증 툴을 직접 구현했습니다.

---

## 2. Focus & Goals

* **시스템 성능 최적화**
  - 초기 벤치마킹 기반의 능동적 Scalability Subsystem 구축 및 HISM(Hierarchical Instanced Static Mesh) 자동 배칭을 통한 Draw Call 절감.

* **에디터 파이프라인 및 데이터 무결성**
   - 에셋 복제로 인한 GUID 충돌 버그를 해결하기 위해 C++ AssetTools 기반의 자동화 툴 및 IsDataValid 정적 검증 시스템 구축.

* **데이터 기반 GAS 아키텍처**
   - 하드코딩을 배제하고 UDataAsset과 FGameplayTag를 활용한 콤보, 스킬, 무기 스왑 로직의 데이터화.

---

## 3. Technical Stack

| 항목 | 내용 |
| :--- | :--- |
| **Engine** | Unreal Engine 5.5 |
| **Language** | C++ (Core Logic), Blueprint (Content Only) |
| **Optimization** | HISM Manager, Unreal Insights, Custom GameInstanceSubsystem |
| **Core Systems** | Gameplay Ability System (GAS), MVVM (UI), Enhanced Input, AI (BT, EQS) |
| **Movement** | Custom CharacterMovement (Climbing), Motion Warping |
| **Editor Pipeline** | AssetTools, DataValidation, Editor Utility Widget |

---

## 4. Game Preview

* (제작중) (기능별 설명 영상)
* https://www.youtube.com/watch?v=4kyv6RzZWCg (인게임 플레이 영상 Full ver)

---

## 5. Core Implementation

### 5.1. Dynamic Scalability & HISM 렌더링 최적화
<details>
<summary><b>상세 내용 보기 (클릭)</b></summary>
  
* **설명:** 과도한 프롭 배치 및 동적 라이트로 인한 GPU/Draw Thread 병목 해소.
* **구현:** 게임 진입 시 2초간의 Warmup 후 5초간 프레임을 측정하여 최적의 그림자/루멘 티어를 결정하는 UJAScalabilitySubsystem 구현. 반복 객체에 대해 메시 스케일 기반 컬링 거리를 계산하는 AJAHISMManager 연동.
* **성과:** 렌더 상태 재생성 억제로 프레임 안정성을 확보하고, 드로우콜 약 71% 감소. 프레임 116% 향상, GPU 119% 가용성 확보.

</details>

### 5.2. 에셋 파이프라인 자동화 (IA 에셋 제너레이터 제작)
<details>
<summary><b>상세 내용 보기 (클릭)</b></summary>

* **설명:** 무기/어빌리티 교체 시 내부 GUID 충돌로 인해 입력 이벤트가 중복 호출되는 엔진 직렬화 이슈 해결.
* **구현:** IsDataValid를 오버라이드하여 데이터 에셋 저장 시 포인터 및 태그 중복을 검사하는 1차 방어선 구축. 이후 C++ AssetTools 모듈과 UI를 결합하여 에셋 생성 및 등록을 자동화하는 제너레이터 툴 제작.
* **성과:** 휴먼 에러로 인한 데이터 오염 원천 차단 및 에셋 파이프라인 생산성 향상.

</details>

### 5.3. 메모리 관리 및 오브젝트 풀링 시스템
<details>
<summary><b>상세 내용 보기 (클릭)</b></summary>
  
* **설명:** 빈번하게 생성/소멸하는 이펙트 및 회피 잔상으로 인한 GC 스파이크 및 메모리 파편화 방지.
* **구현:** 잔상 메시 컴포넌트를 미리 생성해두고 활성/비활성화 상태만 토글하는 오브젝트 풀링 시스템 구축.
* **성과:** 런타임 중 무의미한 액터 스폰 연산을 제거하여 전투 중 프레임 방어 및 메모리 안정성 확보.

</details>

### 5.4. 데이터 기반 GAS 및 전투 시스템
<details>
<summary><b>상세 내용 보기 (클릭)</b></summary>
  
* **설명:** 기획 데이터 수정만으로 컴파일 없이 밸런싱이 가능한 구조 확립.
* **구현:** GameplayAbility를 상속받은 커스텀 베이스 클래스와 UDataAsset, DataTable을 활용하여 총기 및 카타나 근접 콤보, 패링 로직 분기 처리.

</details>

---

## 6. Troubleshooting

### 6.1. Merge Actor 한계 파악 및 HISM 하이브리드 배치
<details>
<summary><b>상세 내용 보기 (클릭)</b></summary>
  
* **문제 상황:** 최적화를 위해 다수의 환경 프롭을 Merge Actor로 정적 병합하였으나, 바운딩 박스가 거대해져 오클루전 컬링이 무력화되고 GPU 연산 부하가 증가함.
* **해결 방법:** 유니크한 배경 구조물은 Merge Actor를 유지하되, 반복되는 프롭은 런타임에 메시를 추출하여 HISM으로 인스턴싱하는 비파괴적 자동 배처 개발.
* **결과:** 오클루전 컬링 효율성 복구 및 Draw Call 극대화. (Unreal Insights를 통해 검증)

</details>

### 6.2. 에셋 직렬화 오염으로 인한 다중 입력 라우팅 이슈 (GUID 충돌)
<details>
<summary><b>상세 내용 보기 (클릭)</b></summary>

* **문제 상황:** 1번 무기 장착 키 입력 시 1번과 2번 장착 어빌리티가 동시 호출되는 현상 발생.
* **분석 및 해결:** 에셋 수동 복제 시 GUID가 갱신되지 않아 쿠킹 환경에서 내부 바인딩 테이블 맵 충돌이 발생함을 디버깅 로그로 입증. 위 [5.2]의 자동화 툴과 검증 로직 도입을 통해 아키텍처 수준에서 재발 방지.

</details>

---

## 7. Project Structure

```text
ProjectJA
 ├── Source
 │   ├── ProjectJA
 │   │   ├── Public / Private
 │   │   │   ├── Character   # Hero, Enemy, Controllers
 │   │   │   ├── GAS         # GA, GE, Attributes, Tasks
 │   │   │   ├── System      # Scalability Subsystem, Instance, AssetManager
 │   │   │   ├── Optimization# HISM Manager
 │   │   │   ├── UI          # CommonUI, Widgets
 │   │   │   └── JAEditor    # Editor Pipeline Tools, Validation
 ├── Content
 │   ├── Blueprints
 │   ├── Maps
 │   └── UI
```

---

## 8. Architecture

### Core System Architecture
> *데이터 파이프라인, 프론트엔드 UI, 코어 전투, 그리고 렌더링 최적화가 유기적으로 결합된 전체 라이프사이클 구조도*

```mermaid
flowchart LR
    %% 1. Editor Pipeline (Data)
    subgraph EditorPipeline [1. 에디터 파이프라인: 데이터 무결성]
        direction TB
        Tool[Input Action Generator] -->|신규 GUID 할당| IA[InputAction]
        IA -->|데이터 에셋 등록| DA_Input[UDataAsset_InputConfig]
        Validator[IsDataValid 검증기] -->|중복 데이터 차단| DA_Input
    end

    %% 2. Frontend Level (Out-game)
    subgraph FrontendLevel [2. 프론트엔드 레벨: Out-Game]
        direction TB
        UI_Front[CommonUI: 프론트엔드 스택] -->|KM & Gamepad 조작| Menu[Options Menu]
        Menu -->|설정 저장| GameInst[Local Settings]
    end

    %% 3. Gameplay Level (In-game)
    subgraph RuntimeCore [3. 게임플레이 레벨: 핵심 전투 시스템]
        direction TB
        Input[Enhanced Input] -->|태그 매핑| ASC[Ability System Component]
        Input -->|이동 로직| CMC[CharacterMovement: Climbing]
        Input -->|입력 라우팅| HUD[CommonUI: HUD]
        
        ASC -->|어빌리티 실행| GA[Gameplay Ability: Weapon/Combo]
        GA -->|데미지/힐 적용| GE[Gameplay Effect]
        GA -->|잔상 생성/회수| Pool[Object Pool: After Image]
        
        ASC -.->|Attribute 브로드캐스트| HUD
    end

    %% 4. Optimization Layer
    subgraph OptimizationLayer [4. 시스템 및 렌더링 최적화]
        direction TB
        Subsystem[JAScalabilitySubsystem] -->|초기화 단계| Bench[초기 성능 벤치마크]
        Bench -->|옵션 티어 세팅| Render[Console Vars: Lumen/Shadow]
        Subsystem -->|월드 로드 감지| HISM[AJAHISMManager]
        HISM -->|메시 인스턴싱| Batch[Draw Call Batching]
    end

    %% Flow Connections (Macro Level)
    EditorPipeline ===>|"안전한 데이터 제공"| RuntimeCore
    FrontendLevel ===>|"Open Level (맵 이동)"| RuntimeCore
    OptimizationLayer ===>|"런타임 렌더링 부하 제어"| RuntimeCore
```


---

## 9. Repository Info

이 저장소는 **클라이언트 아키텍처 설계, 엔진 서브시스템 확장 및 최적화 로직 기록용**입니다.
프로젝트의 핵심 클래스와 엔지니어링 구현 과정을 공개하며, 대용량 에셋 및 빌드 산출물은 포함되지 않습니다.

---

## 10. License / Usage

* **Reference:** 
  * [Udemy – Unreal Engine 5 C++ Advanced Action RPG](https://www.udemy.com/course/unreal-engine-5-advanced-action-rpg-korean/)
  * [Udemy – Unreal Engine 5 C++: 클라이밍 시스템 구축하기](https://www.udemy.com/course/unreal-engine-5-cpp-climbing-system-korean)
  * [Udemy – Unreal Engine 5 C++: Advanced Frontend UI Programming](https://www.udemy.com/course/ureal-engine-5-cpp-advanced-frontend-ui-programming/)


* **Assets:**
  * **배경 리소스**
    * Dwarven City Modular Environment – Scale Z (ArtStation / Fab)

  * **애니메이션**
    * Paragon Character Assets – Epic Games
    * Mixamo - Adobe
    * Unreal Engine Starter Content

  * **VFX 및 사운드**
    * Human Vocalizations – Gamemaster Audio (Epic Games Fab)
    * Realistic Sword Sound Effects – Modern Monkey Studios (Epic Games Fab)
    * Unreal Engine Starter Content
   
  * **로고 및 영상**
    * AI Assisted Design – Gemini (Google)

---

## 11. Author

**Lim Jaehyeok (임재혁)** Game Client Developer
* Email: jehyuk1711@gmail.com
