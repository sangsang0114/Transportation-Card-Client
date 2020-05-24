class SoundPlayer {
  public:
    SoundPlayer();
    void playSound(const byte& id) const;

  private:
    byte fileID;
};
