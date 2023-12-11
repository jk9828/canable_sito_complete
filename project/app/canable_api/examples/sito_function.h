int sito_open(CanInterface* device);
int sito_close(CanInterface* device);

int sito_speed_control(CanInterface* device, int speed);
int sito_current_control(CanInterface* device, int current);
int sito_position_control(CanInterface* device, int position, int speed);
int sito_cease(CanInterface* device);

int sito_get_current(CanInterface* device);
int sito_get_position(CanInterface* device);
int sito_get_speed(CanInterface* device);
int sito_get_temperature(CanInterface* device);

int sito_limit_current(CanInterface* device, int current);
int sito_set_origin(CanInterface* device);

int sito_return_to_origin(CanInterface* device);
