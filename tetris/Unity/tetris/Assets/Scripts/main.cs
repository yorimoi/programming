using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class main : MonoBehaviour {

  [SerializeField]private GameObject BLOCK_WALL;
  [SerializeField]private GameObject BLOCK_TYPE_I;
  [SerializeField]private GameObject BLOCK_TYPE_O;
  [SerializeField]private GameObject BLOCK_TYPE_S;
  [SerializeField]private GameObject BLOCK_TYPE_Z;
  [SerializeField]private GameObject BLOCK_TYPE_J;
  [SerializeField]private GameObject BLOCK_TYPE_L;
  [SerializeField]private GameObject BLOCK_TYPE_T;

  private const byte FIELD_WIDTH    = 10;
  private const byte FIELD_HEIGHT   = 20;

  private const byte MINO_SIZE      = 4;
  private const byte MINO_TYPE_MAX  = 7;
  private const byte MINO_ANGLE_MAX = 4;

  enum state {
    MINO_NONE,
    MINO_I,
    MINO_O,
    MINO_S,
    MINO_Z,
    MINO_J,
    MINO_L,
    MINO_T
  }

  enum type {
    TYPE_I,
    TYPE_O,
    TYPE_S,
    TYPE_Z,
    TYPE_J,
    TYPE_L,
    TYPE_T
  }

  enum angle {
    MINO_ANGLE_0,
    MINO_ANGLE_90,
    MINO_ANGLE_180,
    MINO_ANGLE_270
  }

  int[,,,] mino_AA = new int[MINO_TYPE_MAX, MINO_ANGLE_MAX,
                               MINO_SIZE, MINO_SIZE] {
    // MINO_I
    {
      // MINO_ANGLE_0
      {
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_90
      {
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
      },
      // MINO_ANGLE_180
      {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_270
      {
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
      },
    },
    // MINO_O
    {
      // MINO_ANGLE_0
      {
        { 0, 0, 0, 0},
        { 0, 1, 1, 0},
        { 0, 1, 1, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_90
      {
        { 0, 0, 0, 0},
        { 0, 1, 1, 0},
        { 0, 1, 1, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_180
      {
        { 0, 0, 0, 0},
        { 0, 1, 1, 0},
        { 0, 1, 1, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_270
      {
        { 0, 0, 0, 0},
        { 0, 1, 1, 0},
        { 0, 1, 1, 0},
        { 0, 0, 0, 0},
      },
    },
    // MINO_S
    {
      // MINO_ANGLE_0
      {
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_90
      {
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
      },
      // MINO_ANGLE_180
      {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_270
      {
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
      },
    },
    // MINO_Z
    {
      // MINO_ANGLE_0
      {
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_90
      {
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
      },
      // MINO_ANGLE_180
      {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_270
      {
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
      },
    },
    // MINO_J
    {
      // MINO_ANGLE_0
      {
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_90
      {
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
      },
      // MINO_ANGLE_180
      {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_270
      {
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
      },
    },
    // MINO_L
    {
      // MINO_ANGLE_0
      {
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_90
      {
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
      },
      // MINO_ANGLE_180
      {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_270
      {
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
      },
    },
    // MINO_T
    {
      // MINO_ANGLE_0
      {
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_90
      {
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
        { 0, 0, 1, 0},
      },
      // MINO_ANGLE_180
      {
        { 0, 0, 0, 0},
        { 0, 0, 0, 0},
        { 1, 1, 1, 1},
        { 0, 0, 0, 0},
      },
      // MINO_ANGLE_270
      {
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
        { 0, 1, 0, 0},
      },
    },
  };

  byte[,] field        = new byte[FIELD_HEIGHT, FIELD_WIDTH];
  byte[,] field_buffer = new byte[FIELD_HEIGHT, FIELD_WIDTH];

  byte mino_type;
  byte mino_angle;
  byte mino_x;
  byte mino_y;

  GameObject mino_buffer;

	// Use this for initialization
	void Start () {
    Quaternion rotate = transform.rotation;
    for(int y=2; y<FIELD_HEIGHT+1; y++) {
      Instantiate(BLOCK_WALL, new Vector3( 0, y, 0), rotate);
      Instantiate(BLOCK_WALL, new Vector3( FIELD_WIDTH+1, y, 0), rotate);
    }
    for(int x=0; x<FIELD_WIDTH+2; x++)
      Instantiate(BLOCK_WALL, new Vector3( x, 1, 0), rotate);

    mino_buffer = this.gameObject;

    mino_reset();
	}

	// Update is called once per frame
	void Update () {
    //draw();
	}

  void FixedUpdate() {
    draw();
    if(!cd(mino_x, mino_y-1, mino_type, mino_angle)) mino_y--;
  }

  void draw() {
    foreach (Transform n in mino_buffer.transform)
      GameObject.Destroy(n.gameObject);

    for(int y=0; y<FIELD_HEIGHT; y++)
      for(int x=0; x<FIELD_WIDTH; x++)
        field_buffer[y, x] = field[y, x];
    for(int y=0; y<MINO_SIZE; y++)
      for(int x=0; x<MINO_SIZE; x++)
        if(mino_AA[mino_type-1, mino_angle, y, x] == 1)
          field_buffer[y+mino_y, x+mino_x] = mino_type;

    Quaternion rotate = transform.rotation;
    for(int y=1; y<FIELD_HEIGHT+1; y++) {
      for(int x=1; x<FIELD_WIDTH+1; x++)
        switch((state)field_buffer[y-1, x-1]) {
          case state.MINO_NONE: break;
          case state.MINO_I:
            GameObject now_mino = (GameObject)
              Instantiate(BLOCK_TYPE_I, new Vector3( x, y, 0), rotate);
            now_mino.transform.parent = mino_buffer.transform;
            break;
          case state.MINO_O:
            Instantiate(BLOCK_TYPE_O, new Vector3( x, y, 0), rotate); break;
          case state.MINO_S:
            Instantiate(BLOCK_TYPE_S, new Vector3( x, y, 0), rotate); break;
          case state.MINO_Z:
            Instantiate(BLOCK_TYPE_Z, new Vector3( x, y, 0), rotate); break;
          case state.MINO_J:
            Instantiate(BLOCK_TYPE_J, new Vector3( x, y, 0), rotate); break;
          case state.MINO_L:
            Instantiate(BLOCK_TYPE_L, new Vector3( x, y, 0), rotate); break;
          case state.MINO_T:
            Instantiate(BLOCK_TYPE_T, new Vector3( x, y, 0), rotate); break;
          default:
            break;
        }
    }
  }

  // Collision Detection
  bool cd(int _x, int _y, int _type, int _angle) {
    for(int y=0, y2=3; y<MINO_SIZE; y++, y2--)
      for(int x=0; x<MINO_SIZE; x++)
        if(
          (mino_AA[_type-1, _angle, y2, x] == 1)
          && (field[y+_y, x+_x] != (byte)state.MINO_NONE)
        )
          return true;
    return false;
  }

  void mino_reset() {
    mino_type  = (byte)state.MINO_I;
    mino_angle = 0;
    mino_x     = 3;
    mino_y     = FIELD_HEIGHT - 2;
  }
}
