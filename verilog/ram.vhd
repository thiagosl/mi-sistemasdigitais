----------------------------------------------------------------------------------
-------------------------- Componente memoria ------------------------------------
----------------------------------------------------------------------------------
-- Entrada de configuracao:														--
--  dim - dimensao da memoria (numero de palavras de 32 bits)					--
--																				--
-- Portas de entrada de controle de leitura e escrita nos arquivos:				--
--	read_file  - carrega a memoria com os dados presentes no arquivo "rom.out"	--
--  write_file - carrega no arquivo "data.out" os dados presentes na memoria	--
--																				--
-- Portas de acesso a memoria:													--
--   WE 	- enable de escrita													--
--   clk    - sinal de clock													--
--   ADRESS - endereco de acesso a memoria										--
--	 DATA   - entrada de dados para escrita										--
--   Q      - saida de dados para leitura									    --
----------------------------------------------------------------------------------
--           Componente memoria desenvolvida para a disciplina  			    --
--                      MI -- Sistemas Digitais									--
----------------------------------------------------------------------------------
----------------------------------------------------------------------------------
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;
use STD.TEXTIO.all;
use STD.TEXTIO;
use IEEE.STD_LOGIC_TEXTIO.all;	

entity ram is
	generic (dim : integer := 1024)	;
	port (
		-- bits de controle de escrita no arquivo----
		read_file : in STD_LOGIC;
		write_file : in STD_LOGIC;
		-----------------------------------------------	
		WE : in STD_LOGIC;
		clk: in STD_LOGIC;
		ADRESS : in STD_LOGIC_VECTOR(9 downto 0);	 
		DATA : in STD_LOGIC_VECTOR (31 downto 0);
		Q : out STD_LOGIC_VECTOR (31 downto 0)
	);
end ram;

architecture ram_arch of ram is
type ram_mem_type is array (dim-1 downto 0) of STD_LOGIC_VECTOR (31 downto 0);
signal ram_mem, ram_mem2 : ram_mem_type;
begin
------------------------------------------------------------------------------
-- acesso a memoria ----------------------------------------------------------
------------------------------------------------------------------------------
process (clk)
	variable ADDRwr_TEMP: integer;
	begin
	  if((clk'event) and (clk='1')) then 
		if (read_file = '1') then
			ram_mem <= ram_mem2;
		elsif ((WE = '1') and (not (read_file = '1'))) then 
			ADDRwr_TEMP := CONV_INTEGER(ADRESS); 
				assert(dim > ADDRwr_TEMP)
				     report " Tentou acessar uma posicao de memoria nao definida!"
     					severity ERROR; --FAILURE; --WARNING;
  			ram_mem(ADDRwr_TEMP) <= DATA;
		end if;
	  end if;	
end process;								
Q <= ram_mem(CONV_INTEGER(ADRESS));

-----------------------------------------------------------------------------
--quando read_file transita para '1' o arquivo rom.out e escrito na memoria--
-----------------------------------------------------------------------------
le: process(read_file)
file INFILE : TEXT  is in ".../rom.out"; -- utilize o caminho absoluto para o arquivo
variable DATA_TEMP : STD_LOGIC_VECTOR(31 downto 0);	
variable IN_LINE: LINE;  		
variable index :integer;
begin			 
	if((read_file'event) and (read_file='1')) then
	  index := 0;
	  while NOT(endfile(INFILE)) loop
		readline(INFILE,IN_LINE);	
		hread(IN_LINE, DATA_TEMP);	
		ram_mem2(index) <= DATA_TEMP;
		index := index + 1;
	  end loop;
	end if;		
end process le;	


-------------------------------------------------------------------------------
--quando write_file transita para '1' a memoria e escrita no arquivo data.out--
-------------------------------------------------------------------------------
esc: process( write_file)	
file OUTFILE : TEXT  is out ".../data.out"; -- utilize o caminho absoluto para o arquivo
variable OUT_LINE : LINE;
variable index :integer;
begin
	if((write_file'event) and (write_file='1')) then
	  	index := 0;
		while (index < dim) loop	
--		write(OUT_LINE,index);
--		write(OUT_LINE,":");
		hwrite(OUT_LINE,ram_mem(index));
		writeline(OUTFILE,OUT_LINE);	
		index := index + 1;
	  end loop;
	end if;	
end process esc;	

end ram_arch;



-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------
--  				Arquivo de de teste para o componente ram 					 --
-----------------------------------------------------------------------------------
--   Le os dados do arquivo "rom.out" para a memoria, incrementa de uma unidade  --
--cada valor da memoria e guarda os novos valores da memoria no arquivo data.out --
-----------------------------------------------------------------------------------
--    Arquivo de teste do componente ram desenvolvido para a disciplina de     	 --
--                      MI -- Sistemas Digitais									 --
-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------
library ieee;
use std.textio.all;
use ieee.std_logic_textio.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_1164.all;

entity ram_tb is
		generic(
			dim : INTEGER := 1024 );
end ram_tb;

architecture TB_ARCHITECTURE of ram_tb is
	component ram
		generic(
			dim : INTEGER := 1024 );
		port(
			read_file : in std_logic;
			write_file : in std_logic;
			WE : in std_logic;
			clk : in std_logic;
			ADRESS : in std_logic_vector(9 downto 0);
			DATA : in std_logic_vector(31 downto 0);
			Q : out std_logic_vector(31 downto 0) );
	end component;

	signal read_file : std_logic;
	signal write_file : std_logic;
	signal WE : std_logic;
	signal clk : std_logic;
	signal ADRESS : std_logic_vector(9 downto 0);
	signal DATA : std_logic_vector(31 downto 0);
	signal Q : std_logic_vector(31 downto 0);
begin
	MEMB : ram
		generic map (
			dim => dim
		)
		port map (
			read_file => read_file,
			write_file => write_file,
			WE => WE,
			clk => clk,
			ADRESS => ADRESS,
			DATA => DATA,
			Q => Q
		);

	teste : process	
	variable DATA_TMP : STD_LOGIC_VECTOR(31 downto 0);
	variable index : integer;
	begin  
		write_file <='0';
		read_file <='0';	
		clk <= '0';
		-- le o arquivo rom.out para a memoria 	
		WAIT FOR 1 PS;
		read_file <='1';
		WAIT FOR 1 PS; 
		clk <= '1';		
		WAIT FOR 1 PS;
		read_file <='0';
		clk <= '0';
		WAIT FOR 1 PS;

		index := 0;

		while (index < dim) loop
			-- le a posicao da memoriadada por 'index' e coloca em DATA_TMP
			WE <= '0';
			ADRESS <= CONV_STD_LOGIC_VECTOR(index,10);
			clk <= '1';
			WAIT FOR 1 PS;	
			clk <= '0';		
			WAIT FOR 1 PS;
			DATA_TMP := Q;		
			-- calcula o novo valor a escrever na memoria (incrementa o valor)
			DATA <= DATA_TMP + 1;
			-- escreve a posicao de memoria	dada por 'index'
			WE <= '1';
			WAIT FOR 1 PS;		
			clk <= '1';
			WAIT FOR 1 PS;		
			WE <= '0';
			clk <= '0';	
			WAIT FOR 1 PS;				
			index := index +1;
		end loop;
		-- escreve a memoria no arquivo data.out
		write_file <='1';
		clk <= '1';
		WAIT FOR 1 PS;
		clk <= '0';	  
		write_file <='0';
	 	report "O arquivo data.out foi escrito." severity Warning;
		wait for 10 us;
	end process teste;	
end TB_ARCHITECTURE;

configuration TESTBENCH_FOR_ram of ram_tb is
	for TB_ARCHITECTURE
		for MEMB : ram
			use entity work.ram(ram_arch);
		end for;
	end for;
end TESTBENCH_FOR_ram;